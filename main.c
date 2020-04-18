#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>


int main() 
{
	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO9);
	
	while(1) {
		for (int i = 0; i < 400000; i++) {
			__asm__("nop");
		}
		gpio_toggle(GPIOC, GPIO9);
	}
}
