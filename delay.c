#include <libopencm3/cm3/systick.h>
#include "delay.h"


#define SYSTICK_CLOCK 1000000  // Hz
#define MS_TICKS SYSTICK_CLOCK / 1000


void delay(int duration)
{
	systick_counter_disable();
	systick_interrupt_disable();
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);
	systick_set_reload(MS_TICKS * duration);
	systick_clear();
	systick_get_countflag();
	systick_counter_enable();
	while(!systick_get_countflag());
	systick_counter_disable();
}
