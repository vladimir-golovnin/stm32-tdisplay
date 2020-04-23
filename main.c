#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "delay.h"
#include "lcd1602.h"
#include <stdio.h>


#define LED_BLUE_PORT GPIOC
#define LED_BLUE_PIN GPIO9
#define LED_BLUE_RCC_PERIPH_ID RCC_GPIOC

#define LCD_PORT GPIOB
#define LCD_RCC_PERIPH_ID RCC_GPIOB
#define LCD_E_PIN GPIO11
#define LCD_RS_PIN GPIO10
#define LCD_DB4_PIN GPIO12
#define LCD_DB5_PIN GPIO13
#define LCD_DB6_PIN GPIO14
#define LCD_DB7_PIN GPIO15

#define DISPLAY_CONTRAST_PORT GPIOB
#define DISPLAY_CONTRAST_PIN GPIO8


void board_init() 
{
	rcc_periph_clock_enable(LED_BLUE_RCC_PERIPH_ID);
	rcc_periph_clock_enable(LCD_RCC_PERIPH_ID);

	gpio_set_mode(LED_BLUE_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, LED_BLUE_PIN);

	gpio_set_mode(LCD_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, LCD_E_PIN);
	gpio_set_mode(LCD_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, LCD_RS_PIN);
	gpio_set_mode(LCD_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, LCD_DB4_PIN);
	gpio_set_mode(LCD_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, LCD_DB5_PIN);
	gpio_set_mode(LCD_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, LCD_DB6_PIN);
	gpio_set_mode(LCD_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, LCD_DB7_PIN);

	gpio_set_mode(DISPLAY_CONTRAST_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, DISPLAY_CONTRAST_PIN);

}


lcd_handler_t init_display() 
{
	gpio_clear(DISPLAY_CONTRAST_PORT, DISPLAY_CONTRAST_PIN);
	
	lcd_cfg_t lcd_cfg;
	lcd_cfg.e.port = lcd_cfg.rs.port = lcd_cfg.db4.port = lcd_cfg.db5.port = lcd_cfg.db6.port = lcd_cfg.db7.port = LCD_PORT;
	lcd_cfg.e.pin = LCD_E_PIN;
	lcd_cfg.rs.pin = LCD_RS_PIN; 
	lcd_cfg.db4.pin = LCD_DB4_PIN;
	lcd_cfg.db5.pin = LCD_DB5_PIN;	
	lcd_cfg.db6.pin = LCD_DB6_PIN;
	lcd_cfg.db7.pin = LCD_DB7_PIN;
	lcd_cfg.bus_len = FOUR_BIT;
	lcd_cfg.lines_num = ONE;
	lcd_cfg.font_size = F5_8;
	lcd_handler_t lcd_handler = lcd_init(&lcd_cfg);
	lcd_handler_t *lcd = &lcd_handler;
	lcd_set_display_mode(lcd, true, true, true);
	lcd_set_entry_mode(lcd, RIGHT, false);
	return lcd_handler;
}

void print(lcd_handler_t *lcd, char* str)
{
	while(*str != '\0') {
		lcd_write_data(lcd, *str);
		str++;
	}
}


int main() 
{
	board_init();
	lcd_handler_t lcd = init_display();
	//print(&lcd, "Hi, Vladimir!");
	char str[12];
	sprintf(str, "%i", 423);
	print(&lcd, str);
	delay_ms(5000);
	lcd_clear(&lcd);
	sprintf(str, "%i", 543879828);
	print(&lcd, str);

	while(1) {
		gpio_toggle(GPIOC, GPIO9);
		delay_ms(3000);
	}
}
