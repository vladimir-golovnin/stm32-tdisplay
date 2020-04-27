#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "delay.h"
#include "display.h"
#include <stdio.h>


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
	char str[12];
	sprintf(str, "%i", 543879828);
	write_str(str);

	while(1) {
		gpio_toggle(GPIOC, GPIO9);
		delay_ms(3000);
	}
}
