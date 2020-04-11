// Copyright (c) 2012

#include "myTIMER0.h"

#include <SI32_PBSTD_A_Type.h>
#include <SI32_TIMER_A_Type.h>
#include <si32_device.h>

//==============================================================================
// 2nd Level Interrupt Handlers (Called from generated code)
//==============================================================================
void TIMER0H_high_overflow_handler(void)
{
  SI32_TIMER_A_clear_high_overflow_interrupt(SI32_TIMER_0);

  SI32_PBSTD_A_toggle_pins(SI32_PBSTD_2, 0x0400);

}
