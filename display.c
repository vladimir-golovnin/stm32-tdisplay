#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

#include "lcd1602.h"
#include "display.h"
#include <sys/stat.h>


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
#define DISPLAY_CONTRAST_TIMER TIM16

lcd_handler_t lcd;

void _init_contrast_ctrl() {
        gpio_set_mode(DISPLAY_CONTRAST_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, DISPLAY_CONTRAST_PIN);
	rcc_periph_clock_enable(RCC_TIM16);
	rcc_periph_clock_enable(RCC_AFIO);

	timer_set_oc_mode(DISPLAY_CONTRAST_TIMER, TIM_OC1, TIM_OCM_PWM1);
	timer_set_prescaler(DISPLAY_CONTRAST_TIMER, 0); // 8 MHz / prescaler value
	timer_enable_oc_preload(DISPLAY_CONTRAST_TIMER, TIM_OC1);
	timer_direction_up(DISPLAY_CONTRAST_TIMER);

	timer_set_period(DISPLAY_CONTRAST_TIMER, 999);	
	timer_set_oc_value(DISPLAY_CONTRAST_TIMER, TIM_OC1, 0);
	timer_generate_event(DISPLAY_CONTRAST_TIMER, TIM_EGR_UG);

	timer_enable_break_main_output(DISPLAY_CONTRAST_TIMER);
	timer_enable_oc_output(DISPLAY_CONTRAST_TIMER, TIM_OC1);

	timer_enable_counter(DISPLAY_CONTRAST_TIMER); // must be executed after all
}


void display_init() {
        rcc_periph_clock_enable(LCD_RCC_PERIPH_ID);

        gpio_set_mode(LCD_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, LCD_E_PIN);
        gpio_set_mode(LCD_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, LCD_RS_PIN);
        gpio_set_mode(LCD_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, LCD_DB4_PIN);
        gpio_set_mode(LCD_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, LCD_DB5_PIN);
        gpio_set_mode(LCD_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, LCD_DB6_PIN);
        gpio_set_mode(LCD_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, LCD_DB7_PIN);

	_init_contrast_ctrl();

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

        lcd = lcd_init(&lcd_cfg);
        lcd_set_display_mode(&lcd, true, true, true);
        lcd_set_entry_mode(&lcd, RIGHT, false);
}


void write_char(char ch) {
	if (ch == '\f') lcd_clear(&lcd);
	else lcd_write_data(&lcd, ch);
}


void write_str(char *str) {
	char c;
	while ((c = *str) != '\0') {
		write_char(c);
		str++;
	}
}

#define cmp_step 3
#define cmp_max 300


void set_contrast(uint8_t value) {
	uint8_t contrast_perc = value > 100 ? 100 : value;
	int cmp = cmp_max - (cmp_step * contrast_perc);
	timer_set_oc_value(DISPLAY_CONTRAST_TIMER, TIM_OC1, cmp);
}
