/*----------------------------------------------------------------------------
 *      RL-ARM - USB
 *----------------------------------------------------------------------------
 *      Name:    usbd_xmc4500.c
 *      Purpose: Hardware Layer module for Infineon XMC4500
 *      Rev.:    V4.70
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>
#include <rl_usb.h>
#include <XMC4500.h>

#define __NO_USB_LIB_C
#include "usb_config.c"

#define RX_FIFO_SIZE        256
#define TX0_FIFO_SIZE       64
#define TX1_FIFO_SIZE       64
#define TX2_FIFO_SIZE       64
#define TX3_FIFO_SIZE       64
#define TX4_FIFO_SIZE       64
#define TX5_FIFO_SIZE       64
#define TX6_FIFO_SIZE       64

#define TX_FIFO(n)          *((__packed volatile uint32_t*)(USB0_BASE + 0x1000 + n*0x1000))
#define RX_FIFO             *((volatile uint32_t*)(USB0_BASE + 0x1000))

#define DIEPTSIZ(EPNum)     *(&USB0_EP0->DIEPTSIZ0 + EPNum * 8)
#define DIEPCTL(EPNum)      *(&USB0_EP0->DIEPCTL0  + EPNum * 8)
#define DTXFSTS(EPNum)      *(&USB0_EP0->DTXFSTS0  + EPNum * 8)
#define DOEPTSIZ(EPNum)     *(&USB0_EP0->DOEPTSIZ0 + EPNum * 8)
#define DOEPCTL(EPNum)      *(&USB0_EP0->DOEPCTL0  + EPNum * 8)
#define DIEPINT(EPNum)      *(&USB0_EP0->DIEPINT0  + EPNum * 8)
#define DOEPINT(EPNum)      *(&USB0_EP0->DOEPINT0  + EPNum * 8)

#define EP_IN_TYPE(num)      ((DIEPCTL(num) >> 18) & 3)
#define EP_OUT_TYPE(num)     ((DOEPCTL(num) >> 18) & 3)

uint32_t OutMaxPacketSize[7] = {USBD_MAX_PACKET0,0,0,0,0,0,0};

#if (USBD_HID_ENABLE == 1)
uint32_t HID_IntInPacketData     [(USBD_HID_MAX_PACKET + 3) / 4];
#endif
#if (USBD_CDC_ACM_ENABLE == 1)
uint32_t CDC_ACM_IntInPacketData [(USBD_CDC_ACM_MAX_PACKET  + 3) / 4];
#endif

uint32_t *InPacketDataPtr[7] = { 0,
#if   ((USBD_HID_ENABLE     == 1) && (USBD_HID_EP_INTIN      == 1))
                       HID_IntInPacketData,
#elif ((USBD_CDC_ACM_ENABLE == 1) && (USBD_CDC_ACM_EP_INTIN  == 1))
                       CDC_ACM_IntInPacketData, 
#else
                            0, 
#endif

#if   ((USBD_HID_ENABLE     == 1) && (USBD_HID_EP_INTIN      == 2))
                       HID_IntInPacketData, 
#elif ((USBD_CDC_ACM_ENABLE == 1) && (USBD_CDC_ACM_EP_INTIN  == 2))
                       CDC_ACM_IntInPacketData, 
#else
                            0, 
#endif

#if   ((USBD_HID_ENABLE     == 1) && (USBD_HID_EP_INTIN      == 3))
                       HID_IntInPacketData, 
#elif ((USBD_CDC_ACM_ENABLE == 1) && (USBD_CDC_ACM_EP_INTIN  == 3))
                       CDC_ACM_IntInPacketData, 
#else
                            0, 
#endif

#if   ((USBD_HID_ENABLE     == 1) && (USBD_HID_EP_INTIN      == 4))
                       HID_IntInPacketData, 
#elif ((USBD_CDC_ACM_ENABLE == 1) && (USBD_CDC_ACM_EP_INTIN  == 4))
                       CDC_ACM_IntInPacketData, 
#else
                            0, 
#endif

#if   ((USBD_HID_ENABLE     == 1) && (USBD_HID_EP_INTIN      == 5))
                       HID_IntInPacketData, 
#elif ((USBD_CDC_ACM_ENABLE == 1) && (USBD_CDC_ACM_EP_INTIN  == 5))
                       CDC_ACM_IntInPacketData, 
#else
                            0, 
#endif

#if   ((USBD_HID_ENABLE     == 1) && (USBD_HID_EP_INTIN      == 6))
                       HID_IntInPacketData, 
#elif ((USBD_CDC_ACM_ENABLE == 1) && (USBD_CDC_ACM_EP_INTIN  == 6))
                       CDC_ACM_IntInPacketData, 
#else
                            0, 
#endif
};


#if    (USBD_ADC_ENABLE == 1)

uint32_t ADC_IsoOutPacketData    [(USBD_ADC_WMAXPACKETSIZE + 3) / 4];

uint32_t *IsoOutPacketDataPtr[7] = { 0,
#if    (USBD_ADC_EP_ISOOUT == 1)
                                     ADC_IsoOutPacketData,
#else
                                     0,
#endif
  
#if    (USBD_ADC_EP_ISOOUT == 2)
                                     ADC_IsoOutPacketData,
#else
                                     0,
#endif
  
#if    (USBD_ADC_EP_ISOOUT == 3)
                                     ADC_IsoOutPacketData,
#else
                                     0,
#endif
  
#if    (USBD_ADC_EP_ISOOUT == 4)
                                     ADC_IsoOutPacketData,
#else
                                     0,
#endif

#if    (USBD_ADC_EP_ISOOUT == 5)
                                     ADC_IsoOutPacketData,
#else
                                     0,
#endif

#if    (USBD_ADC_EP_ISOOUT == 6)
                                     ADC_IsoOutPacketData,
#else
                                     0,
#endif
};
#else
uint32_t *IsoOutPacketDataPtr[7] = { 0 };
#endif


uint32_t InPacketDataCnt[7]      = { 0 };
uint32_t InPacketDataReady       =   0  ;
uint32_t SyncWriteEP             =   0  ;

uint32_t IsoOutPacketDataCnt[7]  = { 0 };
uint32_t IsoOutTokenRead         =   0  ;

/*
 *  usbd_xmc4500_delay
 *    Parameters:      delay:      Delay
 *    Return Value:    None
 */
void usbd_xmc4500_delay (uint32_t delay) {
  delay *= SystemCoreClock / 100000;
  while (delay--) {
    __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); __nop();
  }
}


/*
 *  USB Device Interrupt enable
 *   Called by USBD_Init to enable the USB Interrupt
 *    Return Value:    None
 */

#ifdef __RTX
void __svc(1) USBD_IntrEna (void);
void __SVC_1               (void) {
#else
void          USBD_IntrEna (void) {
#endif
  NVIC_EnableIRQ   (USB0_0_IRQn);       /* Enable OTG interrupt               */
}


/*
 *  USB Device Initialize Function
 *   Called by the User to initialize USB
 *   Return Value:    None
 */

void USBD_Init (void) {
  int32_t tout;

  SCU_CLK->CLKSET |= 1;                           /* USB Clock Enable         */
  for (tout=1000; tout>=0; tout--) {              /* Wait                     */
    if (SCU_CLK->CLKSTAT & 1) break;
    if (tout <= 10) usbd_xmc4500_delay(10);       /* for max 100 ms           */
  }

  SCU_RESET->PRCLR2 |=  (1 <<  7);                /* USB Peri Reset Clear     */
  SCU_POWER->PWRSET |=  (1 << 16) | (1 << 17);    /* USB PHY En + OTG En      */

  USB0->GRSTCTL |= USB_GRSTCTL_CSftRst_Msk;       /* USB Core Soft Reset      */
  USB0->GOTGCTL  &= ~(1 << 19);                   /* disable Vbus*            */
  USB0->DCTL     |=  (1 << 1 );                   /* soft disconnect enabled  */
  for (tout=1000; tout>=0; tout--) {              /* Wait                     */
    if (!(USB0->GRSTCTL & USB_GRSTCTL_CSftRst_Msk)) break;
    if (tout <= 100) usbd_xmc4500_delay (10);     /* for max 1000 ms          */
  }

  usbd_xmc4500_delay (100);             /* Wait ~100 ms                       */

  USB0->GUSBCFG  = (1 << 30);           /* force device mode                  */
  usbd_xmc4500_delay (500);

  USBD_IntrEna();                       /* Enable OTG interrupt               */

  USB0->GAHBCFG |= 1 | (1 << 7);
  USB0->GUSBCFG  = (USB0->GUSBCFG & ~(15 << 10)) |(9 << 10);

  USB0->DCFG    |=  3;                /* full speed phy                       */
  USB0->PCGCCTL  =  0;                /* Restart PHY clock                    */

  USB0->GINTMSK_DEVICEMODE  = (1   << 11) | /* suspend int unmask             */
                              (1   << 12) | /* reset int unmask               */
                              (1   << 13) | /* enumeration done int unmask    */
                              (1   << 4 ) | /* recive fifo non-empty int      */
                              (1   << 18) | /* IN EP int unmask               */
                              (1   << 19) | /* OUT EP int unmask              */
                              (1UL << 31) | /* resume int unmask              */
#ifdef __RTX
  ((USBD_RTX_DevTask   != 0) ? (1 <<  3) : 0);   /* SOF int unmask            */
#else
  ((USBD_P_SOF_Event   != 0) ? (1 <<  3) : 0);   /* SOF int unmask            */
#endif
}




/*
 *  USB Device Connect Function
 *   Called by the User to Connect/Disconnect USB Device
 *    Parameters:      con:   Connect/Disconnect
 *    Return Value:    None
 */

void USBD_Connect (uint32_t con) {  
  if (con) {
    USB0->GOTGCTL   =   (1 << 19) |     /* enable Vbus*                       */
                        (1 << 16);      /* power down deactivated             */
    USB0->DCTL     &=  ~(1 << 1 );      /* soft disconnect disabled           */

  } else {
    USB0->GOTGCTL  &= ~(1 << 19);       /* disable Vbus*                      */
    USB0->DCTL     |=  (1 << 1 );       /* soft disconnect enabled            */
  }
}


/*
 *  USB Device Reset Function
 *   Called automatically on USB Device Reset
 *    Return Value:    None
 */

void USBD_Reset (void) {
  uint32_t i;

  SyncWriteEP       = 0;
  InPacketDataReady = 0;
  USB0->DOEPMSK     = 0;
  USB0->DIEPMSK     = 0;

  
  for (i = 0; i < (USBD_EP_NUM + 1); i++) {
    if (DOEPCTL(i) & (1UL << 31))
      DOEPCTL(i) = (1 << 30) | (1 << 27);    /* OUT EP disable, Set NAK       */
    if (DIEPCTL(i) & (1UL << 31))
      DIEPCTL(i) = (1 << 30) | (1 << 27);    /* IN EP disable, Set NAK        */
    
    DIEPINT(i) = 0x1B;                       /* clear IN Ep interrupts        */
    DOEPINT(i) = 0x1B;                       /* clear OUT Ep interrupts       */
  }

  USBD_SetAddress(0 , 1);

  USB0->DAINTMSK = 1 | (1 << 16);       /* unmask IN&OUT EP0 interruts        */

  USB0->DOEPMSK  =  (1 << 3) |          /* setup phase done                   */
                    (1 << 1) |          /* endpoint disabled                  */
                     1;                 /* transfer complete                  */
  USB0->DIEPMSK  =  (1 << 1) |          /* endpoint disabled                  */
                     1;                 /* transfer completed                 */

  USB0->GRXFSIZ  =   RX_FIFO_SIZE/4;
  USB0->HPTXFSIZ =  (RX_FIFO_SIZE/4) | ((TX0_FIFO_SIZE/4) << 16);

  USB0->DIEPTXF1 =  ((RX_FIFO_SIZE + TX0_FIFO_SIZE)/4) |
                    ((TX1_FIFO_SIZE/4) << 16);

  USB0->DIEPTXF2 =  ((RX_FIFO_SIZE + TX0_FIFO_SIZE + TX1_FIFO_SIZE)/4) |
                    ((TX2_FIFO_SIZE/4) << 16);

  USB0->DIEPTXF3 =  ((RX_FIFO_SIZE + TX0_FIFO_SIZE+ TX1_FIFO_SIZE +TX2_FIFO_SIZE)/4) |
                    ((TX3_FIFO_SIZE/4) << 16);

  USB0->DIEPTXF4 =  ((RX_FIFO_SIZE + TX0_FIFO_SIZE+ TX1_FIFO_SIZE +TX2_FIFO_SIZE
                     + TX3_FIFO_SIZE)/4) | ((TX4_FIFO_SIZE/4) << 16);

  USB0->DIEPTXF5 =  ((RX_FIFO_SIZE + TX0_FIFO_SIZE+ TX1_FIFO_SIZE +TX2_FIFO_SIZE
                     + TX3_FIFO_SIZE + TX4_FIFO_SIZE)/4) | ((TX5_FIFO_SIZE/4) << 16);

  USB0->DIEPTXF6 =  ((RX_FIFO_SIZE + TX0_FIFO_SIZE+ TX1_FIFO_SIZE +TX2_FIFO_SIZE
                     + TX3_FIFO_SIZE + TX4_FIFO_SIZE + TX5_FIFO_SIZE)/4) | ((TX6_FIFO_SIZE/4) << 16);

  USB0_EP0->DOEPTSIZ0  =  (1 << 29) |         /* setup count = 1              */
                          (1 << 19) |         /* packet count                 */
                           USBD_MAX_PACKET0;
}


/*
 *  USB Device Suspend Function
 *   Called automatically on USB Device Suspend
 *    Return Value:    None
 */

void USBD_Suspend (void) {
}


/*
 *  USB Device Resume Function
 *   Called automatically on USB Device Resume
 *    Return Value:    None
 */

void USBD_Resume (void) {
}


/*
 *  USB Device Remote Wakeup Function
 *   Called automatically on USB Device Remote Wakeup
 *    Return Value:    None
 */

void USBD_WakeUp (void) {
  USB0->DCTL |= 1;                      /* remote wakeup signaling            */
  usbd_xmc4500_delay (50);              /* Wait ~5 ms                         */
  USB0->DCTL &= ~1;
}


/*
 *  USB Device Remote Wakeup Configuration Function
 *    Parameters:      cfg:   Device Enable/Disable
 *    Return Value:    None
 */

void USBD_WakeUpCfg (uint32_t cfg) {
  /* Not needed                                                               */
}


/*
 *  USB Device Set Address Function
 *    Parameters:      adr:   USB Device Address
 *    Return Value:    None
 */

void USBD_SetAddress (uint32_t  adr, uint32_t setup) {
  if (setup) {
    USB0->DCFG = (USB0->DCFG & ~(0x7f << 4)) | (adr << 4);
  }
}

/*
 *  USB Device Flush IN Endpoint Transmit Fifo
 *    Parameters:      adr:   USB Device Address
 *    Return Value:    None
 */
static void USBD_FlushInEpFifo (uint32_t EPNum) {
  uint32_t wcnt;
  
  EPNum &= ~0x80;
  USB0->GRSTCTL = (USB0->GRSTCTL & ~(0x1F << 6)) | /* flush EP fifo           */
                  (EPNum << 6)| (1 << 5);
  wcnt = 10;
  while (USB0->GRSTCTL & (1 << 5))      /* wait until fifo is flushed         */
    if ((wcnt--) == 0) break;
}


/*
 *  USB Device Configure Function
 *    Parameters:      cfg:   Device Configure/Deconfigure
 *    Return Value:    None
 */

void USBD_Configure (uint32_t cfg) {
  InPacketDataReady &= ~1UL;
}


/*
 *  Configure USB Device Endpoint according to Descriptor
 *    Parameters:      pEPD:  Pointer to Device Endpoint Descriptor
 *    Return Value:    None
 */

void USBD_ConfigEP (USB_ENDPOINT_DESCRIPTOR *pEPD) {
  uint32_t num, val, type;

  num  = pEPD->bEndpointAddress & ~(0x80);
  val  = pEPD->wMaxPacketSize;
  type = pEPD->bmAttributes & USB_ENDPOINT_TYPE_MASK;

  if (pEPD->bEndpointAddress & USB_ENDPOINT_DIRECTION_MASK) {
    USB0->DAINTMSK   |= (1    << num);  /* unmask IN EP int                   */
    DIEPCTL(num)      = (num  <<  22) | /* fifo number                        */
                        (type <<  18) | /* ep type                            */
                         val & 0x7FF;   /* max packet size                    */
    if ((type & 3) > 1)                 /* if interrupt or bulk EP            */
      DIEPCTL(num) |= (1 << 28);        /* DATA0 PID                          */
  } else {
    OutMaxPacketSize[num] = val & 0x7FF;

    USB0->DAINTMSK |= (1 << (num + 16));/* unmask OUT EP int                  */

    DOEPCTL(num)  = (type <<  18)|      /* EP type                            */
                    (val & 0x7FF);      /* max packet size                    */
    DOEPTSIZ(num) = (1 << 19)     |     /* packet count = 1                   */
                    (val  & 0x7FF);     /* transfer size                      */
    if ((type & 3) > 1)                 /* if int or bulk EP                  */
      DOEPCTL(num) |= (1 << 28);        /* DATA0 PID                          */
  }
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
    EPNum &= ~0x80;
    DIEPCTL(EPNum)    |= (1   << 15) |  /* EP active                          */
                         (1   << 27);   /* set EP NAK                         */
    if (DIEPCTL(EPNum)&  (1UL << 31))
      DIEPCTL(EPNum)  |= (1   << 30);   /* disable EP                         */

    InPacketDataReady &= ~(1 << EPNum);
    if (EP_IN_TYPE(EPNum) == USB_ENDPOINT_TYPE_ISOCHRONOUS) {
      USB0->GINTMSK_DEVICEMODE |= (1 << 20);  /* enable IISOIXFR              */
    }
  } else {
    if (EP_OUT_TYPE(EPNum) == USB_ENDPOINT_TYPE_ISOCHRONOUS) {
      USB0->GINTMSK_DEVICEMODE |= (1 << 15);  /* enable end of periodic frame */
      DOEPCTL(EPNum)           |= (1 << 15);  /* EP active                    */
    } else {
      DOEPCTL(EPNum)    |= (1   << 15) |  /* EP active                        */
                           (1UL << 31) |  /* enable EP                        */
                           (1   << 26);   /* clear EP NAK                     */
    }
  }
}


/*
 *  Disable USB Endpoint
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USBD_DisableEP (uint32_t EPNum) {
  uint32_t num, wcnt, IsoEpEnCnt;

  IsoEpEnCnt = 0;

  /* Disable IN Endpoint                                                      */
  if (EPNum & 0x80) {
    EPNum &= ~0x80;
    for (num = 1; num <= USBD_EP_NUM; num++) {
      if (DIEPCTL(num) & (1 <<15)) {
        if (EP_IN_TYPE(num) == USB_ENDPOINT_TYPE_ISOCHRONOUS) {
          IsoEpEnCnt++;
        }
      }
    }

    InPacketDataReady &= ~(1 << EPNum);
    if (IsoEpEnCnt == 1) {              /* if all iso endpoints disabled      */
      USB0->GINTMSK_DEVICEMODE &= ~(1 << 20);  /* disable IISOIXFR            */
    }
    if (DIEPCTL(EPNum) &   (1UL << 31))
      DIEPCTL(EPNum)   |=  (1   << 30); /* disable EP                         */
    DIEPCTL(EPNum)     |=  (1   << 27); /* set EP NAK                         */
    DIEPCTL(EPNum)     &= ~(1   << 15); /* deactivate EP                      */
    
  /* Disable OUT Endpoint                                                     */
  } else {
    for (num = 1; num <= USBD_EP_NUM; num++) {
      if (DOEPCTL(num) & (1 <<15)) {
        if (EP_OUT_TYPE(num) == USB_ENDPOINT_TYPE_ISOCHRONOUS) {
          IsoEpEnCnt++;
        }
      }
    }
    if (IsoEpEnCnt == 1)                /* if all iso endpoints disabled      */
      USB0->GINTMSK_DEVICEMODE &= ~(1 << 15); /* disable EOPF                 */
    
    USB0->DCTL |= (1 << 9);                   /* set global out nak           */

    wcnt = 1000;
    while (!(USB0->GINTSTS_DEVICEMODE & (1 << 7)))  /* wait until global NAK  */
      if ((wcnt--) == 0) break;
    
    if (DOEPCTL(EPNum) &   (1UL << 31))   /* if ep enabled                    */
      DOEPCTL(EPNum)   |=  (1   << 30);   /* disable EP                       */
    DOEPCTL(EPNum)     |=  (1   << 27);   /* set EP NAK                       */
    DOEPCTL(EPNum)     &= ~(1   << 15);   /* deactivate EP                    */

    wcnt = 1000;
    while(!(DOEPINT(EPNum) & (1 << 1)))   /* wait until EP disabled           */
      if ((wcnt--) == 0) break;
    USB0->DCTL |= (1 << 10);              /* clear global nak                 */
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

  /* Reset IN Endpoint                                                        */
  if (EPNum & 0x80) {
    EPNum &= ~0x80;
    InPacketDataReady &= ~(1 << EPNum);
    if (DIEPCTL(EPNum) &  (1UL << 31))  /* if Endpoint enabled                */
      DIEPCTL(EPNum)   |= (1   << 30);  /* disable EP                         */
    DIEPCTL(EPNum)     |= (1   << 27);  /* set EP NAK                         */

    USBD_FlushInEpFifo (EPNum | 0x80);  /* Flush endpoint fifo                */

    /* If endpoint is isochronous, set proper EVEN/ODD frame and enable Ep    */
    if (EP_IN_TYPE(EPNum) == USB_ENDPOINT_TYPE_ISOCHRONOUS) {
      if (USBD_GetFrame() & 1) DIEPCTL(EPNum) |= (1 << 28); /* set even frame */
      else DIEPCTL(EPNum) |= (1 << 29);                     /* set odd frame  */
      DIEPCTL(EPNum)  |= (1UL << 31) | (1 << 26);           /* enable EP      */
    }
  }
}


/*
 *  Set Stall for USB Device Endpoint
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USBD_SetStallEP (uint32_t EPNum) {
  uint32_t wcnt;
  
  /* Stall OUT Endpoint                                                       */
  if (!(EPNum & 0x80)) {
    USB0->DCTL |= (1 << 9);             /* set global out nak                 */
    wcnt = 1000;
    while (!(USB0->GINTSTS_DEVICEMODE & (1 << 7)))  /* wait until global NAK  */
      if ((wcnt--) == 0) break;
    
    if (DOEPCTL(EPNum) &   (1UL << 31)) /* if endpoint enabled                */
      DOEPCTL(EPNum)   |=  (1   << 30); /* disable EP                         */
    DOEPCTL(EPNum) |= (1 << 21);        /* set stall                          */
    
    wcnt = 1000;
    while(!(DOEPINT(EPNum) & (1 << 1))) /* wait until EP disabled             */
      if ((wcnt--) == 0) break;
      
    USB0->DCTL |= (1 << 10);            /* clear global nak                   */

  /* Stall IN endpoint                                                        */
  } else {
    EPNum &= ~0x80;
    if (DIEPCTL(EPNum) &  (1UL << 31))  /* if endpoint enabled                */
      DIEPCTL(EPNum)   |= (1   << 30);  /* disable endpoint                   */
    DIEPCTL(EPNum)     |= (1   << 21);  /* set stall                          */

    USBD_FlushInEpFifo (EPNum | 0x80);  /* Flush endpoint fifo                */
  }
}


/*
 *  Clear Stall for USB Device Endpoint
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USBD_ClrStallEP (uint32_t EPNum) {
  /* Clear OUT endpoint Stall                                                 */
  if (!(EPNum & 0x80)) {

    if (((DOEPCTL(EPNum) >> 18) & 3) > 1) { /* if EP type Bulk or Interrupt   */
      DOEPCTL(EPNum) |= (1 << 28);      /* Set DATA0 PID                      */

    DOEPCTL(EPNum) &= ~(1 << 21);       /* Clear stall                        */
  }
 
  /* Clear IN Endpoint Stall                                                  */
  } else {
    EPNum &= ~0x80;

    if (DIEPCTL(EPNum) &  (1UL << 31))  /* if endpoint enabled                */
      DIEPCTL(EPNum)   |= (1 << 30);    /* disable endpoint                   */

    USBD_FlushInEpFifo (EPNum | 0x80);  /* Flush endpoint fifo                */

    if (((DIEPCTL(EPNum) >> 18) & 3) > 1)/* if Interrupt or bulk EP           */
      DIEPCTL(EPNum) |= (1 << 28);      /* Set DATA0 PID                      */

    DIEPCTL(EPNum) &= ~(1 << 21);       /* clear Stall                        */
  }
}


/*
 *  Clear USB Device Endpoint Buffer
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USBD_ClearEPBuf (uint32_t EPNum) {
  if (EPNum & 0x80) {
    USBD_FlushInEpFifo (EPNum | 0x80);
  } else {
    USB0->GRSTCTL |= (1 << 4);
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
  }
}


/*
 *  Read USB Device Endpoint Data
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *                     pData: Pointer to Data Buffer
 *    Return Value:    Number of bytes read
 */

uint32_t USBD_ReadEP (uint32_t EPNum, uint8_t *pData) {
  uint32_t i, sz, isoEpFlag;

  if ((DOEPCTL(EPNum) & (1 << 15)) == 0) return (0); /* if Ep not active      */
  isoEpFlag = (EP_OUT_TYPE(EPNum) == USB_ENDPOINT_TYPE_ISOCHRONOUS);
  
  /* Isochronous Ep: read data from intermediate buffer                       */
  if (isoEpFlag) {                      /* if Isochronouos endpoint           */
    if (IsoOutTokenRead == 0) {          
      sz = IsoOutPacketDataCnt[EPNum];  /* get data size                      */
      for (i = 0; i < sz; i++) {        /* copy data from intermediate buffer */
        pData[i] = ((uint8_t *)(IsoOutPacketDataPtr[EPNum]))[i];
      }
      IsoOutPacketDataCnt[EPNum] = 0;   /* reset data count                   */
      return (sz);
    }
  }
  
  sz = ( USB0->GRXSTSP_DEVICEMODE >> 4) & 0x7FF; /* get available data size   */
  if (isoEpFlag) {
    /* if data pid = data0 and 
       number of packets in which this payload was received = 1, data is valid*/
    if ((DOEPTSIZ(EPNum) & ((0x3FF << 19) | (3 << 29))) == 0)  
      IsoOutPacketDataCnt[EPNum] = sz;  /* save isochronous Ep data size      */
    else IsoOutPacketDataCnt[EPNum] = 0;/* else data is unvalid               */ 
  }

  /* copy data from fifo
     if Isochronous Ep: data is copied to intermediate buffer                 */
  for (i = 0; i < (uint32_t)((sz+3)/4); i++) {
    *((__packed uint32_t *)pData) = RX_FIFO;
    pData += 4;
  }
  /* wait RxFIFO non-empty (OUT transfer completed or Setup trans. completed) */
  while ((USB0->GINTSTS_DEVICEMODE & (1 << 4)) == 0);  
  USB0->GRXSTSP_DEVICEMODE;             /* pop register                       */
  USB0->GINTMSK_DEVICEMODE |= (1 << 4); /* unmask RxFIFO non-empty interrupt  */

  return (sz);
}


/*
 *  Write USB Device Endpoint Data
 *  If write was requested synchronously from IRQ then data is written to FIFO directly 
 *  else data is written to the intermediate buffer and synchronously transferred to FIFO 
 *  on next NAK event.
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *                     pData: Pointer to Data Buffer
 *                     cnt:   Number of bytes to write
 *    Return Value:    Number of bytes written
 */

uint32_t USBD_WriteEP (uint32_t EPNum, uint8_t *pData, uint32_t cnt) {
  uint32_t *ptr, i, isoEpFlag;

  EPNum &= ~(0x80);

  if ((DIEPCTL(EPNum) & (1 << 15)) == 0) return (0); /* if Ep not active      */
  isoEpFlag = (EP_IN_TYPE(EPNum) == USB_ENDPOINT_TYPE_ISOCHRONOUS);

  /* Asynchronous write to intermediate buffer                                */
  if (!SyncWriteEP && InPacketDataPtr[EPNum]) {
    if ((!(InPacketDataReady & (1 << EPNum))) || isoEpFlag) {
      InPacketDataCnt[EPNum] = cnt;     /* save Data size                     */
      ptr = InPacketDataPtr[EPNum];
      i   = (cnt+3)/4;
      if (i) { 
        while (i--) {                   /* save data to intermediate buffer   */
          *ptr++ = *((__packed uint32_t *)pData);
          pData +=4;
        }
      }
      if (!isoEpFlag) {
        InPacketDataReady |=  1 << EPNum;
        DIEPCTL(EPNum)    |= (1 << 27); /* Set NAK to enable interrupt on NAK */
        USB0->DIEPMSK     |= (1 <<  6); /* INEPNEM = 1, IN EP NAK efective msk*/
      }
    } else {                            /* If packet already loaded to buffer */
      return 0;
    }
  } else {
    if (cnt) while ((DTXFSTS(EPNum) * 4) < cnt); /* get space in Ep TxFIFO    */

    /* Set transfer size and packet count                                     */
    DIEPTSIZ(EPNum) = cnt | (1 << 19) | (1 << 29); 
                             
    if (isoEpFlag) {                    /* if Isochronous Ep: set packet frame*/
      if (USBD_GetFrame() & 1) DIEPCTL(EPNum) |= (1 << 28); /* even frame     */
      else DIEPCTL(EPNum) |= (1 << 29);                     /* odd frame      */
    }
    
    DIEPCTL(EPNum) |= (1UL << 31) | (1 << 26);  /* enable ep and clear NAK    */
    if (cnt) {
      ptr = (uint32_t *)pData;
      i   = (cnt+3)/4;
      while (i--) {                     /* copy data to endpoint TxFIFO       */
        TX_FIFO(EPNum) = *(__packed uint32_t *)pData;
        pData +=4;
      }
    }
    if (isoEpFlag == 0) InPacketDataReady &= ~(1 << EPNum);
  }
  return (cnt);
}


/*
 *  Get USB Device Last Frame Number
 *    Parameters:      None
 *    Return Value:    Frame Number
 */

uint32_t USBD_GetFrame (void) {
  return ((USB0->DSTS >> 8) & 0x3FFF);
}


/*
 *  USB Device Interrupt Service Routine
 */
void USB0_0_IRQHandler(void) {
  uint32_t istr, val, num, i, msk;
  static uint32_t IsoInIncomplete = 0;

  istr = USB0->GINTSTS_DEVICEMODE & USB0->GINTMSK_DEVICEMODE;

/* reset interrupt                                                            */
  if (istr & (1 << 12)) {
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
    USB0->GINTSTS_DEVICEMODE = (1 << 12);
  }

/* suspend interrupt                                                          */
  if (istr & (1 << 11)) {
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
    USB0->GINTSTS_DEVICEMODE = (1 << 11);
  }

/* resume interrupt                                                           */
  if (istr & (1UL << 31)) {
    USBD_Resume();
#ifdef __RTX
    if (USBD_RTX_DevTask) {
      isr_evt_set(USBD_EVT_RESUME, USBD_RTX_DevTask);
    }
#else
    if (USBD_P_Resume_Event) {
      USBD_P_Resume_Event();
    }
#endif
    USB0->GINTSTS_DEVICEMODE = (1UL << 31);
  }

/* speed enumeration completed                                                */
  if (istr & (1 << 13)) {
    USB0_EP0->DIEPCTL0 &= ~(3UL);
    switch (USBD_MAX_PACKET0) {
      case 8:
        USB0_EP0->DIEPCTL0 |= 3;
      break;

      case 16:
        USB0_EP0->DIEPCTL0 |= 2;
      break;

      case 32:
        USB0_EP0->DIEPCTL0 |= 1;
      break;
    }
    USB0->DCTL    |= (1 << 8);          /* clear global IN NAK                */
    USB0->DCTL    |= (1 << 10);         /* clear global OUT NAK               */
    USB0->GINTSTS_DEVICEMODE = (1 << 13);
  }

/* Start Of Frame                                                             */
  if (istr & (1 << 3)) {
#ifdef __RTX
    if (USBD_RTX_DevTask) {
      isr_evt_set(USBD_EVT_SOF, USBD_RTX_DevTask);
    }
#else
    if (USBD_P_SOF_Event) {
      USBD_P_SOF_Event();
    }
#endif
    USB0->GINTSTS_DEVICEMODE = (1 << 3);
  }



/* RxFIFO non-empty                                                           */
  if (istr & (1 << 4)) {
    val = USB0->GRXSTSR_DEVICEMODE;
    num = val & 0x0F;

    switch ((val >> 17) & 0x0F) {
/* setup packet                                                               */
      case 6:
#ifdef __RTX
      USB0->GINTMSK_DEVICEMODE &= ~(1 << 4);
      if (USBD_RTX_EPTask[num]) {
        isr_evt_set(USBD_EVT_SETUP, USBD_RTX_EPTask[num]);
      }
#else
      if (USBD_P_EP[num]) {
        USBD_P_EP[num](USBD_EVT_SETUP);
      }
#endif
      break;

/* OUT packet                                                                 */
      case 2:
      USB0->GINTMSK_DEVICEMODE &= ~(1 << 4);
      if (EP_OUT_TYPE(num) == USB_ENDPOINT_TYPE_ISOCHRONOUS) {
        IsoOutTokenRead = 1;
        USBD_ReadEP (num, (uint8_t *)IsoOutPacketDataPtr[num]);
        IsoOutTokenRead = 0;
      } else {
#ifdef __RTX
        if (USBD_RTX_EPTask[num]) {
          isr_evt_set(USBD_EVT_OUT, USBD_RTX_EPTask[num]);
        }
#else
        if (USBD_P_EP[num]) {
          USBD_P_EP[num](USBD_EVT_OUT);
        }
#endif
      }
      break;

      default:
      USB0->GRXSTSP_DEVICEMODE;
    }
  }

/* OUT Packet                                                                 */
  if (istr & (1 << 19)) {
    msk = (((USB0->DAINT & USB0->DAINTMSK) >> 16) & 0xFFFF);
    i   = 0;
    while (msk) {
      num = 0;
      for (; i < (USBD_EP_NUM+1); i++) {
        if ((msk >> i) & 1) {
          num = i;
          msk &= ~(1 << i);
          break;
        }
      }

      /* Endpoint disabled                                                    */
      if (DOEPINT(num) & (1 << 1)) {
        if (EP_OUT_TYPE(num) == USB_ENDPOINT_TYPE_ISOCHRONOUS) {
          DOEPTSIZ(num) = (1 << 19) |                /* packet count          */
                          (OutMaxPacketSize[num]);   /* transfer size         */
          
          if ((USBD_GetFrame() & 1)) DOEPCTL(num) |= (1 << 28); /* even frame */
          else                       DOEPCTL(num) |= (1 << 29); /* odd frame  */

          DOEPCTL(num)    |= (1UL <<31) | (1 << 26);
        }
        DOEPINT(num) |= (1 << 1);
      }
      
      /* Transfer complete interrupt                                          */
      if ((DOEPINT(num) & 1) | (DOEPINT(num) & (1 << 3))) {
        if (EP_OUT_TYPE(num) != USB_ENDPOINT_TYPE_ISOCHRONOUS) {

          DOEPTSIZ(num)    = (1 << 19) |                 /* packet count      */
                             (OutMaxPacketSize[num]);    /* transfer size     */
          if (num == 0) {
            DOEPTSIZ(0)   |= (1 << 29);
          }
          DOEPCTL(num)    |= (1UL <<31) | (1 << 26);     /* clr NAK, en EP    */
        }
        DOEPINT(num) |= 1;
      }
    }
  }

/* IN Packet                                                                  */
  if (istr & (1 << 18)) {
    msk = (USB0->DAINT & USB0->DAINTMSK & 0xFFFF);
    i   = 0;
    while (msk) {
      num = 0;
      for (; i < (USBD_EP_NUM+1); i++) {
        if ((msk >> i) & 1) {
          num = i;
          msk &= ~(1 << i);
          break;
        }
      }

      /* Endpoint disabled                                                    */
      if (DIEPINT(num) & (1 << 1)) {
        DIEPINT(num) = (1 << 1);
        
        if (EP_IN_TYPE(num) == USB_ENDPOINT_TYPE_ISOCHRONOUS) {
          if ((IsoInIncomplete & (1 << num)) != 0) {

            USBD_FlushInEpFifo(num | 0x80);
            SyncWriteEP = 1;
            USBD_WriteEP (num, (uint8_t *)InPacketDataPtr[num], InPacketDataCnt[num]);
            SyncWriteEP = 0;

            IsoInIncomplete &= ~(1 << num);
          }
        }
      }

      /* IN endpoint NAK effective                                            */
      if (DIEPINT(num) & (1 << 6)) {
        if (EP_IN_TYPE(num) != USB_ENDPOINT_TYPE_ISOCHRONOUS) {
          if (InPacketDataPtr[num] && (InPacketDataReady & (1 << num))) {
            SyncWriteEP = 1;
            USBD_WriteEP (num, (uint8_t *)InPacketDataPtr[num], InPacketDataCnt[num]);
            SyncWriteEP = 0;
            if (!InPacketDataReady)       /* No more pending IN transfers     */
              USB0->DIEPMSK &= ~(1 << 6); /* Disable IN NAK interrupts        */
            continue;
          } else 
              DIEPCTL(num) |= (1 << 26);
            DIEPINT(num)    = (1 <<  6);
        }
      }

      /* Transmit completed                                                   */
      if (DIEPINT(num) & 1) {
        DIEPINT(num) = 1;
        SyncWriteEP = 1;
        if (EP_IN_TYPE(num) == USB_ENDPOINT_TYPE_ISOCHRONOUS) {
          USBD_WriteEP (num, (uint8_t *)InPacketDataPtr[num], InPacketDataCnt[num]);
        } else {
#ifdef __RTX
          if (USBD_RTX_EPTask[num]) {
            isr_evt_set(USBD_EVT_IN,  USBD_RTX_EPTask[num]);
          }
#else
          if (USBD_P_EP[num]) {
            USBD_P_EP[num](USBD_EVT_IN);
          }
#endif
        }
        SyncWriteEP = 0;
      }
    }
  }
  
/* End of periodic frame                                                      */
  if (istr & (1 << 15)) {
    for (num = 1; num <= USBD_EP_NUM; num++) {

      if (EP_OUT_TYPE(num) != USB_ENDPOINT_TYPE_ISOCHRONOUS) continue;
      if (((DOEPCTL(num) >> 15) & 1) == 0)                   continue;

      /* incomplete isochronous out transfer                                  */
      if (USB0->GINTSTS_DEVICEMODE & (1 << 21)) {
        if ((USBD_GetFrame() & 1) == ((DOEPCTL(num) >> 16) & 1)) {
          if (DOEPCTL(num) & (1UL << 31)) {
            InPacketDataCnt[num] = 0;   /* discard data                       */
            DOEPCTL(num) |= (1UL << 30);/* disable endpoint                   */
          }
        }

      /* prepare for next isohronous transfer                                 */
      } else {
        DOEPTSIZ(num) = (1 << 19) |                /* packet count            */
                        (OutMaxPacketSize[num]);   /* transfer size           */

        if ((USBD_GetFrame() & 1)) DOEPCTL(num) |= (1 << 28); /* even frame   */
        else                       DOEPCTL(num) |= (1 << 29); /* odd frame    */

        DOEPCTL(num)    |= (1UL <<31) | (1 << 26);
      }
    }
    USB0->GINTSTS_DEVICEMODE = (1 << 15) | (1 << 21);
  }

/* incomplete isochronous IN transfer                                         */
  if (istr & (1 << 20)) {
    USB0->GINTSTS_DEVICEMODE = (1 << 20);
    for (num = 1; num < (USBD_EP_NUM + 1); num++) {

      if (EP_IN_TYPE(num) != USB_ENDPOINT_TYPE_ISOCHRONOUS) continue;
      if (((DIEPCTL(num) >> 15) & 1) == 0)                  continue;

      if (DIEPCTL(num) & (1UL << 31)) { /* if EP en & packet frame is incorect*/
        if ((USBD_GetFrame() & 1) == ((DIEPCTL(num) >> 16) & 1)) {
          
          IsoInIncomplete |= (1 << num);
          DIEPCTL(num)    |= (1UL << 30) | (1 << 27);
        }
      }
    }
  }
}
