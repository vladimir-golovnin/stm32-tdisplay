#include "delay.h"
#include "lcd1602.h"
#include <libopencm3/stm32/gpio.h>


#define CMD_DISPLAY_CLR 1
#define CMD_ENTRY_MODE 4
#define CMD_DISPLAY_MODE 8
#define CMD_FUNC_SET 0x20


void _set_pin(gpio_pin_t pin, bool state)
{
	if (state) gpio_set(pin.port, pin.pin);
	else gpio_clear(pin.port, pin.pin);
}


void _write_nibble(uint8_t data, lcd_handler_t *lcd)
{
	_set_pin(lcd->db4, data & 1);
	_set_pin(lcd->db5, data & 2);
	_set_pin(lcd->db6, data & 4);
	_set_pin(lcd->db7, data & 8);
	delay_us(2);
	gpio_set(lcd->e.port, lcd->e.pin);
	delay_us(2);
	gpio_clear(lcd->e.port, lcd->e.pin);
}


void _write_byte(uint8_t data, lcd_handler_t *lcd)
{
	_write_nibble(data >> 4, lcd);
	_write_nibble(data, lcd);
}


void _write_cmd(lcd_handler_t *lcd, uint8_t cmd_code)
{
	gpio_clear(lcd->rs.port, lcd->rs.pin);
	_write_byte(cmd_code, lcd);
	delay_us(40);
}


void lcd_write_data(lcd_handler_t *lcd, uint8_t data)
{
	gpio_set(lcd->rs.port, lcd->rs.pin);
	_write_byte(data, lcd);
	delay_us(43);
}


void lcd_clear(lcd_handler_t *lcd)
{
	_write_cmd(lcd, CMD_DISPLAY_CLR);
	delay_us(1500);
}


void lcd_set_entry_mode(lcd_handler_t *lcd, cursor_mv_dir_t mv_dir, bool display_shift)
{
	uint8_t move_mode_code = mv_dir == RIGHT ? 2 : 0;
	uint8_t shift_code = display_shift ? 1 : 0;
	uint8_t cmd = CMD_ENTRY_MODE | move_mode_code | shift_code;
	_write_cmd(lcd, cmd);
} 


void lcd_set_display_mode(lcd_handler_t *lcd, bool display_on, bool cursor_on, bool cursor_blinking)
{
	uint8_t display_state_code = display_on ? 4 : 0;
	uint8_t cursor_state_code = cursor_on ? 2 : 0;
	uint8_t cursor_blink_code = cursor_blinking ? 1 : 0;
	uint8_t cmd = CMD_DISPLAY_MODE | display_state_code | cursor_state_code | cursor_blink_code;
	_write_cmd(lcd, cmd);
}


// 8 bit bus length is not supported yet.
void _func_set(lcd_handler_t *lcd, bus_len_t bus_len, lines_num_t lines_num, font_size_t font_size)
{
	uint8_t lines = lines_num == ONE ? 0 : 8;
	uint8_t font = font_size == F5_10 ? 4 : 0;
	uint8_t	cmd = CMD_FUNC_SET | lines | font;
	_write_cmd(lcd, cmd);
}


lcd_handler_t lcd_init(lcd_cfg_t *cfg)
{
	lcd_handler_t lcd = {cfg->e, cfg->rs, cfg->db4, cfg->db5, cfg->db6, cfg->db7};
	gpio_clear(lcd.e.port, lcd.e.pin);
	gpio_clear(lcd.rs.port, lcd.rs.pin);
	delay_ms(15);
	// 4-bit mode
	_write_nibble(2, &lcd);

	_func_set(&lcd, cfg->bus_len, cfg->lines_num, cfg->font_size);

	return lcd;
}
