#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>


#define DS1621_CFG_DONE 0x80
#define DS1621_CFG_1SHOT 1
#define DS1621_CFG_CONT 0

typedef struct {
	uint8_t address;
        void (*transfer)(uint8_t, uint8_t*, size_t, uint8_t*, size_t);	
} i2c_handler_t;


uint8_t ds1621_read_config(const i2c_handler_t *h);
void ds1621_set_config(const i2c_handler_t *h, uint8_t config);
bool ds1621_convert_done(const i2c_handler_t *h);
void ds1621_start_convert(const i2c_handler_t *h);
uint16_t ds1621_read_temperature(const i2c_handler_t *h);


