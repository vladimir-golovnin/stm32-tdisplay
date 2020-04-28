#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "delay.h"
#include "display.h"
#include <stdio.h>
#include "tsensor.h"


#define LED_BLUE_PORT GPIOC
#define LED_BLUE_PIN GPIO9
#define LED_BLUE_RCC_PERIPH_ID RCC_GPIOC

void board_init() 
{
	rcc_periph_clock_enable(LED_BLUE_RCC_PERIPH_ID);
	gpio_set_mode(LED_BLUE_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, LED_BLUE_PIN);
}


int main() 
{
	board_init();
	display_init();
	set_contrast(100);

	write_str("Hello!");
	
	if (tsensor_init()) write_str("\fNo sensor!");
	else {
		int t;
		char str[16];
		while(1) {
			delay_ms(2000);
			t = tsensor_get_temp();
			sprintf(str, "\fTemp: %i\xEF" "C", t);
			write_str(str);
			gpio_toggle(LED_BLUE_PORT, LED_BLUE_PIN);
		}
	}

	delay_ms(1000);
	write_str("\fGood bye.");
	while(1); 
}
