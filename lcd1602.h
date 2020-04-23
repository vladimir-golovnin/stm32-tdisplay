#include <stdint.h>
#include <stdbool.h>


typedef struct {
	uint32_t port;
	uint16_t pin;
} gpio_pin_t;


typedef enum {RIGHT, LEFT} cursor_mv_dir_t;
typedef enum {FOUR_BIT, EIGHT_BIT} bus_len_t;
typedef enum {ONE, TWO} lines_num_t;
typedef enum {F5_10,F5_8} font_size_t;


typedef struct {
	gpio_pin_t e;
	gpio_pin_t rs;
	gpio_pin_t db4;
	gpio_pin_t db5;
	gpio_pin_t db6;
	gpio_pin_t db7;
	bus_len_t bus_len;
	lines_num_t lines_num;
	font_size_t font_size;
} lcd_cfg_t;

typedef struct {
	gpio_pin_t e;
	gpio_pin_t rs;
	gpio_pin_t db4;
	gpio_pin_t db5;
	gpio_pin_t db6;
	gpio_pin_t db7;
} lcd_handler_t;


lcd_handler_t lcd_init(lcd_cfg_t *cfg);
void lcd_write_data(lcd_handler_t *lcd, uint8_t data);
void lcd_set_entry_mode(lcd_handler_t *lcd, cursor_mv_dir_t mv_dir, bool display_shift);
void lcd_set_display_mode(lcd_handler_t *lcd, bool display_on, bool cursor_on, bool cursor_blinking);
void lcd_clear(lcd_handler_t *lcd);
