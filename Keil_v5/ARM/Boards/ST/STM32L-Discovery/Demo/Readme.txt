/**
    @page Demo Demo
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    STM32L-Discovery/readme.txt 
  * @author  Microcontroller Division
  * @version V1.0.1
  * @date    June-2011
  * @brief   Description for STM32L-Discovery board demonstration software.
  ******************************************************************************
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
   @endverbatim

@par Hardware and Software environment

- This Demo runs with MDK-ARM from version 4.20 and onwards.

  - Software environemnt
  ----------------------
     - MDK-ARM 4.20 should be installed to support the embedded STLINK/V2.
     
  - ST-LINK/V2 driver requirement
  ------------------------------
    - First install the ST-LINK/V2 driver by running the ST-Link_V2_USB.exe from
      [MDK-ARM_INSTALL_DIRECTORY]\ARM\STLink\USBDriver\ST-Link_V2_USBdriver.exe
        
  - Hardware environemnt
  ----------------------
     - Check jumper position on the board, JP1 on, CN3 on (Discovery selected).
     - Connect the STM32L-Discovery board to a PC with a USB cable to power the board.

@par Example Description 

This software demonstration aims to use various peripherals and connectors on
STM32L-Discovery board.

When powering on the STM32L-Discovery board, the Red LED LD2 (PWR) then lights up.
Function 1 (see table below) is executed, and each click on user button B1 changes 
the executed function.
|--------------------------------------------------------------------------------
| Function |  LED LD3/4	  |  Description	              | MAIN FUNCTION        |
|----------|--------------|-------------------------------|----------------------|
|   1	   | Blink LD3/4  | STM32L VDD voltage measured	  |Voltage               |
|----------|--------------|-------------------------------|----------------------|
|   2	   |    LD3	      | Slider 0 to 100%	          |Slider                |
|----------|--------------|-------------------------------|----------------------|
|   3	   |    LD4	      | Used as 4 touch buttons	      |                      |
|----------|--------------|-------------------------------|                      |
|   4	   |    Off	      | Run mode (4 MHz)	          |      STM32L          |
|	       |	          | Sleep mode (4 MHz)	          |                      |
|----------|--------------|-------------------------------|    consumption       |
|   5	   |     Off	  | Low power mode, Run (32 kHz   |                      |
|    	   |              | Low power mode, Sleep (32 kHz)|     measured         |
|----------|--------------|-------------------------------|                      |
|   6	   |     Off	  | Stop mode with RTC            |                      |
|          |              | Stop mode without RTC         |                      |
|----------|--------------|-------------------------------|                      |
|   7	   |     Off	  | Standby mode  	              |                      |
|          |              |                               |                      |
---------------------------------------------------------------------------------|
	


@par How to use it ? 

In order to make the program work, you must do the following :
1- Run the ST-Link_V2_USBdriver.exe (detailed in "ST-LINK/V2 driver requirement" section above)
2- Open STM32L-Discovery project
3- Rebuild all files and start the debugger which will also load your image into target flash memory
4- End the debug session and press reset on the board to run the example without debugger

   
 * <h3><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h3>
 */

