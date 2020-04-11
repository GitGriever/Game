/*----------------------------------------------------------------------------
 *      RL-ARM - USB
 *----------------------------------------------------------------------------
 *      Name:    usbd_tmpm36x.c
 *      Purpose: Hardware Layer module for Toshiba TMPM36x Series
 *      Rev.:    V4.70
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>
#include <rl_usb.h>
#include <TMPM369.H>
#include <string.h>

#define __NO_USB_LIB_C
#include "usb_config.c"
#include "usbd_hw.h"

#define EP_NUM                  (8)   /*EP0 - EP7*/

#define USBD_INTSTATUS_MASK      (uint32_t)0x23FE07FFU
#define USBD_INTMASK_UDC2AB      (uint32_t)0x23FE0700U

#define USBD_POWERCTRL_MASK      (uint32_t)0x00FFU

#define REQUEST_SET_ADDRESS      (uint32_t)0x05U

#define STATE_ADDRESSED          (uint32_t)0x0200U
#define STATE_CONFIGURED         (uint32_t)0x0400U


#define EP_MAXPACKETSIZE_OFFSET  (uint32_t)(0x0U)
#define EP_STATUS_OFFSET         (uint32_t)(0x4U)
#define EP_DATASIZE_OFFSET       (uint32_t)(0x8U)
#define EP_FIFO_OFFSET           (uint32_t)(0xCU)

static uint32_t Usb_Adr          = (uint32_t)0x0U;
static BOOL setupflag            = FALSE;
#ifdef __RTX
static BOOL isr_flag             = FALSE;
#endif
static uint8_t  OutEpSize[EP_NUM];

static uint32_t const EpBaseAdd[EP_NUM] = 
{
  UDC2_EP0MAXPACKETSIZE, UDC2_EP1MAXPACKETSIZE, UDC2_EP2MAXPACKETSIZE, UDC2_EP3MAXPACKETSIZE,
  UDC2_EP4MAXPACKETSIZE, UDC2_EP5MAXPACKETSIZE, UDC2_EP6MAXPACKETSIZE, UDC2_EP7MAXPACKETSIZE,
};


/*
 *  USB Device Interrupt enable
 *   Called by USBD_Init to enable the USB Interrupt
 *    Return Value:    None
 */
#ifdef __RTX
void __svc(1) USBD_IntrEna (BOOL enable);
void __SVC_1               (BOOL enable) {
#else
void USBD_IntrEna (BOOL enable) {
#endif
  if (enable) NVIC_EnableIRQ(INTUSBD_IRQn);                 /* Enable USB Device interrupt               */
  else        NVIC_DisableIRQ(INTUSBD_IRQn);                /* Disable USB Device interrupt               */
}


/*
 * Read data from UDC2 registers which must be read through UDC2 Read Request Register of UDC2AB
 * Addr: The address of registers in UDC2 module.
 * Data:  The pointer point to where the value of register will be stored.
 * Return Value:    None
 */
static __forceinline void USBD_ReadUDC2Reg(uint32_t Addr, uint32_t * Data)
{
  volatile uint32_t tmp = 0U;

  /* Set 8 bits(<9:2>) of 10bits offset address */
  /* Set bit31 to issue a read request          */
  TSB_UDFS->UDC2RDREQ = (Addr & (uint32_t) 0x03FCU) | (0x1U << 31U);
    
  while (TSB_UDFS_UDC2RDREQ_UDC2RDREQ) {              /* Wait till udc2rdreq turn to 0 */
  };          
  
  *Data = TSB_UDFS->UDC2RDVL & (uint32_t) (0xFFFFU);  /* Get result and only keep low 16bits */
}

/*
 * Write data to UDC2 registers.
 * Addr: The address of registers in UDC2 module.
 * Data: The data which will be written to UDC2 register.
 * Return Value:    None
 */

static __forceinline void USBD_WriteUDC2Reg(uint32_t Addr, const uint32_t Data)
{
  volatile uint32_t *tmp_addr = 0U;

  tmp_addr = (uint32_t *) Addr;
  *tmp_addr = Data & (uint32_t) (0xFFFFU);       /* Write low 16 bits */

  while (TSB_UDFS_UDC2RDREQ_UDC2RDREQ) {         /* Wait till udc2rdreq turn to 0 */
  };

}

/*
 * Clear the specified interrupt flag of USBD.
 * IntSrc: Specify the interrupt source.
 * Return Value:    None
 */
void USBD_ClearINT(USBD_INTSrc IntSrc)
{
  uint32_t tmp = 0U;
  if (IntSrc <= USBD_INT_NAK) {
    USBD_ReadUDC2Reg(UDC2_INT, &tmp);
    tmp |= 0x01UL << (uint32_t) IntSrc;      /* Set bit to clear UDC2 Interrupt */
    USBD_WriteUDC2Reg(UDC2_INT, tmp);
  } else {                                   /* Set bit to clear the specified UDC2AB Interrupt */
    tmp |= 0x01UL << (uint32_t) IntSrc;
    TSB_UDFS->INTSTS = tmp;
  }
}
/*
 * Set mask of the specified interrupt of USBD.
 * IntSrc: Specify the interrupt source.
 * NewState: Set the interrupt source mask state.
 * Return Value:    None
 */
void USBD_SetINTMask(USBD_INTSrc IntSrc, FunctionalState NewState)
{
  uint32_t tmp = 0U;
  
  if (IntSrc <= USBD_INT_NAK) {
    USBD_ReadUDC2Reg(UDC2_INT, &tmp);
    if (NewState == ENABLE) {
      tmp &= (uint32_t) (~(0x01UL << ((uint32_t) IntSrc + 8U)));  /* Clear bit to enable UDC2 interrupt     */
    } else {
      tmp |= 0x01UL << ((uint32_t) IntSrc + 8U);                  /* Set bit to disable UDC2 interrupt      */
    }
    USBD_WriteUDC2Reg(UDC2_INT, tmp);
  } else {
    tmp = TSB_UDFS->INTENB & USBD_INTMASK_UDC2AB;
    if (NewState == ENABLE) {                                     /* Set bit to enable UDC2AB interrupt     */
      tmp |= 0x01UL << (uint32_t) IntSrc;
    } else {                                                      /* Clear bit to disable UDC2AB interrupt  */
      tmp &= (uint32_t) (~(0x01UL << (uint32_t) IntSrc));
    }
    TSB_UDFS->INTENB = tmp;
  }
}

/*
 * Send command to endpoints.
 * EPx: Specify the target endpoint (x = 0 to 7).
 * Cmd: The command which will be sent to endpoint
 * Return Value:    None
 */

void USBD_SetEPCMD(USBD_EPx EPx, USBD_EPCMD Cmd)
{
  uint32_t tmp = 0U;
#ifdef __RTX
  if (!isr_flag) USBD_IntrEna(FALSE);
#endif
  USBD_ReadUDC2Reg(UDC2_COMMAND, &tmp);
  tmp &= (uint32_t) (0x1U << 15U);                             /* keep int_toggle bit */
  tmp |= (uint32_t)((EPx << 4U) | Cmd);
  USBD_WriteUDC2Reg(UDC2_COMMAND, tmp);
#ifdef __RTX
  if (!isr_flag) USBD_IntrEna(TRUE);
#endif
}


/*
 *  USB Device Initialize Function
 *   Called by the User to initialize USB
 *   Return Value:    None
 */
uint32_t gDelay = 0x20000U;
void usbd_wait_1ms(void)
{
    uint32_t delay = 0U;

    delay = gDelay;
    while (delay) {
        delay--;
    }

}

void USBD_Init (void) {
  USBD_PowerCtrl pwr = { 0U };

  TSB_USBPLL->SEL        =  USBD_INPUT_16M;                      /* Input clock 16MHz, output clock 48MHz for USB */
  usbd_wait_1ms();
  TSB_USBPLL_CR_USBPLLON =              1U;                      /* USBPLL operation: Oscillation                 */ 
  usbd_wait_1ms();
  TSB_USBPLL_EN_USBDEN   =              1U;                      /* 48MHz clock for USB device: On                */ 
  
  /* Set GPIO PE0 as Output (PE0 controls the pull up of D+) */
  TSB_PE_FR1_PE1F1   =   TSB_PE_FR2_PE1F2   =   TSB_PE_FR3_PE1F3   = \
  TSB_PE_FR4_PE1F4   =   TSB_PE_FR5_PE1F5   =   TSB_PE_IE_PE0IE    = 0x0U;
  TSB_PE_CR_PE0C     =   0x1U;
  
  TSB_PE_DATA_PE0    =   0x1U;                                   /* Pull up on D+                   */   
  
  USBD_SetINTMask(USBD_INT_USB_RESET_END, ENABLE);
  USBD_SetINTMask(USBD_INT_USB_RESET, ENABLE);        
  
  /*  UDPWCTL Power Reset and            */
  /*      PHY Reset & Suspend: 1ms       */
  pwr.All = TSB_UDFS->PWCTL & USBD_POWERCTRL_MASK;
  pwr.Bit.PW_Resetb      =     0U;
  pwr.Bit.PHY_Resetb     =     0U;
  pwr.Bit.PHY_Suspend    =     1U;
  TSB_UDFS->PWCTL = pwr.All & USBD_POWERCTRL_MASK;
  usbd_wait_1ms();
  
  /* PHY Reset off : 1ms                  */
  pwr.All = TSB_UDFS->PWCTL & USBD_POWERCTRL_MASK;
  pwr.Bit.PHY_Resetb = 1U;
  pwr.Bit.PHY_Suspend = 1U;
  TSB_UDFS->PWCTL = pwr.All & USBD_POWERCTRL_MASK;
  usbd_wait_1ms();
  
  /*  PHY Suspend off : 1ms              */
  pwr.All = TSB_UDFS->PWCTL & USBD_POWERCTRL_MASK;
  pwr.Bit.PHY_Suspend = 0U;
  TSB_UDFS->PWCTL = pwr.All & USBD_POWERCTRL_MASK;
  usbd_wait_1ms();
  
  usbd_wait_1ms();
  
  /*  UDPWCTL Power Reset Off: 1ms        */
  pwr.All = TSB_UDFS->PWCTL & USBD_POWERCTRL_MASK;
  pwr.Bit.PW_Resetb = 1U;
  TSB_UDFS->PWCTL = pwr.All & USBD_POWERCTRL_MASK;
  usbd_wait_1ms();
  usbd_wait_1ms();
  pwr.All = TSB_UDFS->PWCTL & USBD_POWERCTRL_MASK;
  
  
  USBD_WriteUDC2Reg(UDC2_INT, 0x90FFU);                         /* Clear pending UDC2 interrupt and disable INT for SOF and NAK */
  USBD_SetEPCMD(USBD_EP0, USBD_CMD_USB_READY);                  /* Connect to USB                            */ 

  NVIC_EnableIRQ(INTUSBD_IRQn);                                 /* Enable USB Device interrupt               */
}


/*
 *  USB Device Connect Function
 *   Called by the User to Connect/Disconnect USB Device
 *    Parameters:      con:   Connect/Disconnect
 *    Return Value:    None
 */

void USBD_Connect (uint32_t con) {

  if (con) {
    TSB_USBPLL_EN_USBDEN   =  0x1U;                             /* 48MHz clock for USB device: On  */ 
    TSB_PE_DATA_PE0        =  0x1U;                             /* Pull up on D+                   */   
  }
  else {
    TSB_USBPLL_EN_USBDEN   =  0x1U;                             /* 48MHz clock for USB device: Off */ 
    TSB_PE_DATA_PE0        =  0x0U;                             /* pull down on D+                 */ 
  }
}


/*
 *  USB Device Reset Function
 *   Called automatically on USB Device Reset
 *    Return Value:    None
 */

void USBD_Reset (void) {
  int i;

  Usb_Adr        =     (uint32_t)0x0U;
  setupflag      =              FALSE;

  for (i = 0; i < EP_NUM; i++)
  {
    OutEpSize[i] = 0;
  }

  /* Initialize Control Endpoint */
  OutEpSize[0] = USBD_MAX_PACKET0;
  USBD_WriteUDC2Reg((uint32_t)(EpBaseAdd[0]+EP_MAXPACKETSIZE_OFFSET), OutEpSize[0]);
  USBD_WriteUDC2Reg((uint32_t)(EpBaseAdd[0]+EP_STATUS_OFFSET), 0x00);
  USBD_SetEPCMD(USBD_EP0, USBD_CMD_EP_FIFO_CLEAR);
}


/*
 *  USB Device Suspend Function
 *   Called automatically on USB Device Suspend
 *    Return Value:    None
 */

void USBD_Suspend (void) {
  /* Done by HW                                                               */
}


/*
 *  USB Device Resume Function
 *   Called automatically on USB Device Resume
 *    Return Value:    None
 */

void USBD_Resume (void) {
  /* Done by HW                                                               */
}


/*
 *  USB Device Remote Wakeup Function
 *   Called automatically on USB Device Remote Wakeup
 *    Return Value:    None
 */

void USBD_WakeUp (void) {
  /* Done by HW                                                               */
}


/*
 *  USB Device Remote Wakeup Configuration Function
 *    Parameters:      cfg:   Device Enable/Disable
 *    Return Value:    None
 */

void USBD_WakeUpCfg (uint32_t cfg) {
}


/*
 *  USB Device Set Address Function
 *    Parameters:      adr:   USB Device Address
 *    Return Value:    None
 */
void USBD_SetAddress (uint32_t  adr, uint32_t setup) {
  if (!setup) {
#ifdef __RTX
    if (!isr_flag) USBD_IntrEna(FALSE);
#endif
    Usb_Adr = adr;
    USBD_WriteUDC2Reg(UDC2_ADDR, (adr & 0x7F) | STATE_ADDRESSED);
#ifdef __RTX
    if (!isr_flag) USBD_IntrEna(TRUE);
#endif
  }
}


/*
 *  USB Device Configure Function
 *    Parameters:      cfg:   Device Configure/Deconfigure
 *    Return Value:    None
 */

void USBD_Configure (uint32_t cfg) {
  
  if (cfg) {
#ifdef __RTX
    if (!isr_flag) USBD_IntrEna(FALSE);
#endif
    USBD_WriteUDC2Reg(UDC2_ADDR, (Usb_Adr & 0x7F) | STATE_CONFIGURED);
#ifdef __RTX
    if (!isr_flag) USBD_IntrEna(TRUE);
#endif
    USBD_SetEPCMD(USBD_EP0, USBD_CMD_ALL_EP_INVALID);
  }

}


/*
 *  Configure USB Device Endpoint according to Descriptor
 *    Parameters:      pEPD:  Pointer to Device Endpoint Descriptor
 *    Return Value:    None
 */

void USBD_ConfigEP (USB_ENDPOINT_DESCRIPTOR *pEPD) {
  uint32_t num, val, type;

  num  = pEPD->bEndpointAddress & 0x7F;
  val  = pEPD->wMaxPacketSize;
  type = pEPD->bmAttributes & USB_ENDPOINT_TYPE_MASK;

  OutEpSize[num] = val;
#ifdef __RTX
  if (!isr_flag) USBD_IntrEna(FALSE);
#endif
  USBD_WriteUDC2Reg((uint32_t)(EpBaseAdd[num]+EP_MAXPACKETSIZE_OFFSET), OutEpSize[num]);
  USBD_WriteUDC2Reg((uint32_t)(EpBaseAdd[num]+EP_STATUS_OFFSET), \
    (pEPD->bEndpointAddress & USB_ENDPOINT_DIRECTION_MASK) | (type << 2));
#ifdef __RTX
  if (!isr_flag) USBD_IntrEna(TRUE);
#endif
  USBD_SetEPCMD((USBD_EPx)num, USBD_CMD_EP_RESET);
  USBD_SetEPCMD((USBD_EPx)num, USBD_CMD_EP_ENABLE);
}


/*
 *  Set Direction for USB Device Control Endpoint
 *    Parameters:      dir:   Out (dir == 0), In (dir <> 0)
 *    Return Value:    None
 */

void USBD_DirCtrlEP (uint32_t dir) {
  /* Not needed                                                               */
}


/*
 *  Enable USB Device Endpoint
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USBD_EnableEP (uint32_t EPNum) {
  if (EPNum & 0x80) {
    EPNum &= 0x0F;
    USBD_SetEPCMD((USBD_EPx) EPNum, USBD_CMD_EP_ENABLE);
  }
  else {
    USBD_SetEPCMD((USBD_EPx) EPNum, USBD_CMD_EP_ENABLE);  
  }
}


/*
 *  Disable USB Endpoint
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USBD_DisableEP (uint32_t EPNum) 
{
  if (EPNum & 0x80) {
    EPNum &= 0x0F;
    USBD_SetEPCMD((USBD_EPx) EPNum, USBD_CMD_EP_DISABLE);
  }
  else {
    USBD_SetEPCMD((USBD_EPx) EPNum, USBD_CMD_EP_DISABLE);
  }
}


/*
 *  Reset USB Device Endpoint
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USBD_ResetEP (uint32_t EPNum) {
  EPNum &= 0x0F; 
  USBD_SetEPCMD((USBD_EPx) EPNum, USBD_CMD_EP_RESET);
}

/*
 *  Set Stall for USB Device Endpoint
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USBD_SetStallEP (uint32_t EPNum) {
  EPNum &= 0x0F; 
  USBD_SetEPCMD((USBD_EPx)EPNum, USBD_CMD_EP_STALL);
}


/*
 *  Clear Stall for USB Device Endpoint
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USBD_ClrStallEP (uint32_t EPNum) {
  EPNum &= 0x0F; 
  USBD_ResetEP (EPNum);
}


/*
 *  Clear USB Device Endpoint Buffer
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USBD_ClearEPBuf (uint32_t EPNum) {
  EPNum &= 0x0F; 
  USBD_SetEPCMD((USBD_EPx)EPNum, USBD_CMD_EP_FIFO_CLEAR);
}


/*
 *  Read USB Device Endpoint Data
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *                     pData: Pointer to Data Buffer
 *    Return Value:    Number of bytes read
 */
USB_SETUP_PACKET setup_packet;

uint32_t USBD_ReadEP (uint32_t EPNum, uint8_t *pData) {
  uint32_t n, sz, idx, tmp;
  uint16_t * idx16 = (uint16_t *)pData;	
#ifdef __RTX
  if (!isr_flag) USBD_IntrEna(FALSE);
#endif
  if (setupflag == TRUE && EPNum == 0) {
    idx = 0;
     /* get Device Requests Data */
    USBD_ReadUDC2Reg(UDC2_BREQ_BMREQTYPE, &tmp);
    idx16[idx++] = (uint16_t) (tmp & 0x0000FFFFU);
    USBD_ReadUDC2Reg(UDC2_WVALUE, &tmp);
    idx16[idx++] = (uint16_t) (tmp & 0x0000FFFFU);
    USBD_ReadUDC2Reg(UDC2_WINDEX, &tmp);
    idx16[idx++] = (uint16_t) (tmp & 0x0000FFFFU);
    USBD_ReadUDC2Reg(UDC2_WLENGTH, &tmp);
    idx16[idx++] = (uint16_t) (tmp & 0x0000FFFFU);
    memcpy((void *)&setup_packet, (void *)idx16, sizeof(USB_SETUP_PACKET));
    
    /* recognize Setup Stage (Received Device Request) */
    USBD_SetEPCMD(USBD_EP0, USBD_CMD_SETUP_RECEIVED);
    setupflag = FALSE;
    return(8);
  }
  else {
    USBD_ReadUDC2Reg((uint32_t)(EpBaseAdd[EPNum]+EP_DATASIZE_OFFSET), &sz);
    n = sz;
    while (n != 0U) {
      USBD_ReadUDC2Reg((uint32_t)(EpBaseAdd[EPNum]+EP_FIFO_OFFSET), &tmp);
      if (n == 1U) {
         *(uint8_t *) idx16 = (uint8_t) tmp;
         n = 0U;
      } else {
         *idx16 = (uint16_t) tmp;
         idx16++;
         n -= 2U;       /* size of uint16_t */
      }
    }
  }
#ifdef __RTX
  if (!isr_flag) USBD_IntrEna(TRUE);
#endif
  return (sz);
}


/*
 *  Write USB Device Endpoint Data
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *                     pData: Pointer to Data Buffer
 *                     cnt:   Number of bytes to write
 *    Return Value:    Number of bytes written
 */
uint32_t USBD_WriteEP (uint32_t EPNum, uint8_t *pData, uint32_t cnt) {
  uint32_t n;
  uint16_t *idx16 = (uint16_t *)pData;
  EPNum &=0x0F;
  n = cnt;
#ifdef __RTX
  if (!isr_flag) USBD_IntrEna(FALSE);
#endif
  while (n != 0U) {
    if (n == 1U) {
      /* write data to endpoint0's FIFO */
      *((volatile uint8_t *) ((uint32_t)(EpBaseAdd[EPNum]+EP_FIFO_OFFSET))) = (uint8_t) (*idx16);
      /* Wait till udc2rdreq turn to 0 */
      while (TSB_UDFS_UDC2RDREQ_UDC2RDREQ) {
      };
      n = 0U;
    } else {
      USBD_WriteUDC2Reg((uint32_t)(EpBaseAdd[EPNum]+EP_FIFO_OFFSET), (uint32_t) (*idx16));
      idx16++;
      n -= 2U;   /* size of uint16 */
    }
  }
  if (cnt < OutEpSize[EPNum]) {
    if (cnt) USBD_SetEPCMD((USBD_EPx)EPNum, USBD_CMD_EP_EOP);
    else {
      USBD_SetEPCMD((USBD_EPx)EPNum, USBD_CMD_EP_TX_0DATA);
      USBD_SetEPCMD((USBD_EPx)EPNum, USBD_CMD_EP_EOP);
    }
  }
#ifdef __RTX
  if (!isr_flag) USBD_IntrEna(TRUE);
#endif
  return(cnt);
}

/*
 *  Get USB Device Last Frame Number
 *    Parameters:      None
 *    Return Value:    Frame Number
 */

uint32_t USBD_GetFrame (void) {
  return(0);
}

#ifdef __RTX
U32 LastError;                          /* Last Error                         */

/*
 *  Get USB Device Last Error Code
 *    Parameters:      None
 *    Return Value:    Error Code
 */

U32 USBD_GetError (void) {
  return (LastError);
}
#endif


/*
 *  USB Device Interrupt Service Routine
 */

void INTUSBD_IRQHandler(void) {
  uint32_t num, cnt;
  volatile USBD_INTStatus istr = { 0U };
  volatile uint32_t int_ep;
#ifdef __RTX  
  isr_flag = TRUE;
#endif
  istr.All = TSB_UDFS->INTSTS & USBD_INTSTATUS_MASK;

  if (istr.Bit.Setup) {                            /* SETUP interrupt */
    USBD_ClearINT(USBD_INT_SETUP);
    setupflag = TRUE;
#ifdef __RTX
    if (USBD_RTX_EPTask[0]) {
      isr_evt_set(USBD_EVT_SETUP, USBD_RTX_EPTask[0]);
    }
#else
    if (USBD_P_EP[0]) {
      USBD_P_EP[0](USBD_EVT_SETUP);
    }
#endif
  }
  else if (istr.Bit.Status_NAK) {                  /* STATUS NAK interrupt */
    USBD_ClearINT(USBD_INT_STATUS_NAK);
    USBD_SetEPCMD(USBD_EP0, USBD_CMD_SETUP_FIN);
  }
  else if (istr.Bit.Status) {                      /* STATUS interrupt */
    USBD_ClearINT(USBD_INT_STATUS);
    if (setup_packet.bmRequestType.Dir == 1) { /*Host To Device*/
#ifdef __RTX
      if (USBD_RTX_EPTask[0]) {
        isr_evt_set(USBD_EVT_OUT, USBD_RTX_EPTask[0]);
      }
#else
      if (USBD_P_EP[0]) {
        USBD_P_EP[0](USBD_EVT_OUT);
      }
#endif
    }
    else {
#ifdef __RTX
      if (USBD_RTX_EPTask[0]) {
        isr_evt_set(USBD_EVT_IN,  USBD_RTX_EPTask[0]);
      }
#else
      if (USBD_P_EP[0]) {
        if (setup_packet.bRequest == REQUEST_SET_ADDRESS) {
          USBD_P_EP[0](USBD_EVT_IN);
        }
      }
#endif
    }
  }
  else if (istr.Bit.Rx_Zero) {                     /* RX_ZERO interrupt */
    USBD_ReadUDC2Reg(UDC2_INTRX0, (uint32_t *)&num);
    for(cnt = 1; (EP_NUM > cnt) && num; ++cnt)
    {
      if ((1UL<<cnt) & num)
      {
        USBD_WriteUDC2Reg(UDC2_INTRX0,(1UL<<cnt));
        if ((cnt & 0x1) & num) {          /* Endpoint 2/4/6 (OUT) */
#ifdef __RTX
          if (USBD_RTX_EPTask[cnt]) {
            isr_evt_set(USBD_EVT_IN,  USBD_RTX_EPTask[cnt]);
          }
#else
          if (USBD_P_EP[cnt]) {
            USBD_P_EP[cnt](USBD_EVT_IN);
          }
#endif
        }
        else  {                           /* Endpoint 2/4/6 (OUT)  */
#ifdef __RTX
          if (USBD_RTX_EPTask[cnt]) {
            isr_evt_set(USBD_EVT_OUT, USBD_RTX_EPTask[cnt]);
          }
#else
          if (USBD_P_EP[cnt]) {
            USBD_P_EP[cnt](USBD_EVT_OUT);
          }
#endif
        }
      }
    }
  }
  if (istr.Bit.SOF) {                              /* SOF interrupt */
    USBD_ClearINT(USBD_INT_SOF);
#ifdef __RTX
    if (USBD_RTX_DevTask) {
      isr_evt_set(USBD_EVT_SOF, USBD_RTX_DevTask);
    }
#else
    if (USBD_P_SOF_Event) {
      USBD_P_SOF_Event();
    }
#endif
  }
  else if (istr.Bit.EP0) {                         /* EP0 interrupt */
    USBD_ClearINT(USBD_INT_EP0);
    if (setup_packet.bmRequestType.Dir == 0) { /*Host To Device*/
#ifdef __RTX
      if (USBD_RTX_EPTask[0]) {
        isr_evt_set(USBD_EVT_OUT, USBD_RTX_EPTask[0]);
      }
#else
      if (USBD_P_EP[0]) {
        USBD_P_EP[0](USBD_EVT_OUT);
      }
#endif
    }
    else {
#ifdef __RTX
      if (USBD_RTX_EPTask[0]) {
        isr_evt_set(USBD_EVT_IN,  USBD_RTX_EPTask[0]);
      }
#else
      if (USBD_P_EP[0]) {
        USBD_P_EP[0](USBD_EVT_IN);
      }
#endif
    }
  }
  else if (istr.Bit.EP) {                          /* EPx interrupt */
    USBD_ReadUDC2Reg(UDC2_INTEP, (uint32_t *)&num);
    for(cnt = 1; (EP_NUM > cnt) && num; ++cnt)
    {
      if ((1UL<<cnt) & num)
      {
        USBD_WriteUDC2Reg(UDC2_INTEP,(1UL<<cnt));
        if (cnt & 0x1) {                 /* Endpoint 1/3/5/7 (IN) */
#ifdef __RTX
          if (USBD_RTX_EPTask[cnt]) {
            isr_evt_set(USBD_EVT_IN,  USBD_RTX_EPTask[cnt]);
          }
#else
          if (USBD_P_EP[cnt]) {
            USBD_P_EP[cnt](USBD_EVT_IN);
          }
#endif
        }
        else  {                          /* Endpoint 2/4/6 (OUT)  */
#ifdef __RTX
          if (USBD_RTX_EPTask[cnt]) {
            isr_evt_set(USBD_EVT_OUT, USBD_RTX_EPTask[cnt]);
          }
#else
          if (USBD_P_EP[cnt]) {
            USBD_P_EP[cnt](USBD_EVT_OUT);
          }
#endif
        }
      }
    }

  }
  else if (istr.Bit.NAK) {                         /* NAK interrupt           */
    USBD_ClearINT(USBD_INT_NAK);
  }
  else if (istr.Bit.USB_Reset) {                   /* RESET interrupt         */
    USBD_ClearINT(USBD_INT_USB_RESET);
  }
  else if (istr.Bit.USB_Reset_End) {               /* RESET_END interrupt     */
    USBD_ClearINT(USBD_INT_USB_RESET_END);
    USBD_Reset();
    usbd_reset_core();
#ifdef __RTX
    if (USBD_RTX_DevTask) {
      isr_evt_set(USBD_EVT_RESET, USBD_RTX_DevTask);
    }
#else
    if (USBD_P_Reset_Event) {
      USBD_P_Reset_Event();
    }
#endif
  }
  else if (istr.Bit.Suspend_Resume) {              /* SUSPEND_RESUME interrupt */
    USBD_ClearINT(USBD_INT_SUSPEND_RESUME);
    USBD_Suspend();
#ifdef __RTX
      if (USBD_RTX_DevTask) {
        isr_evt_set(USBD_EVT_SUSPEND, USBD_RTX_DevTask);
      }
#else
      if (USBD_P_Suspend_Event) {
        USBD_P_Suspend_Event();
      }
#endif
  }
#ifdef __RTX
  isr_flag = FALSE;
#endif
}

