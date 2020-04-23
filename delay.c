#include <libopencm3/cm3/systick.h>
#include "delay.h"


#define SYSTICK_CLOCK 1000000  // Hz
#define MS_TICKS   SYSTICK_CLOCK / 1000
#define US_TICKS   SYSTICK_CLOCK / 1000000


void _wait_ticks(int ticks_num)
{
	systick_counter_disable();
	systick_interrupt_disable();
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);
	systick_set_reload(ticks_num);
	systick_clear();
	systick_get_countflag();
	systick_counter_enable();
	while(!systick_get_countflag());
	systick_counter_disable();
}

void delay_ms(int duration)
{
	_wait_ticks(duration * MS_TICKS);
}


void delay_us(int duration)
{
	_wait_ticks(duration * US_TICKS);	
}
