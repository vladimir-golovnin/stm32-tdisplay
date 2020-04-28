#include "ds1621.h"


// Commands
#define CMD_ACCESS_CONFIG 0xAC
#define CMD_READ_TEMP 0xAA
#define CMD_START_CONVERT 0xEE

#define ADDRESS_PREFIX 0x48
#define ADDRESS_MASK 7
#define _addr_byte(addr) (addr & ADDRESS_MASK | ADDRESS_PREFIX)


uint8_t ds1621_read_config(const i2c_handler_t *h) {
	uint8_t cmd = CMD_ACCESS_CONFIG;
	uint8_t config;
	uint8_t addr = _addr_byte(h->address);
	h->transfer(addr, &cmd, sizeof(cmd), &config, sizeof(config));
	return config;
}


bool ds1621_convert_done(const i2c_handler_t *h) {
	return ds1621_read_config(h) & DS1621_CFG_DONE;
}


void ds1621_set_config(const i2c_handler_t *h, uint8_t config) {
	uint8_t cmd[] = {CMD_ACCESS_CONFIG, config};
	uint8_t addr = _addr_byte(h->address);
	h->transfer(addr, cmd, sizeof(cmd), NULL, 0);
}


void ds1621_start_convert(const i2c_handler_t *h) {
	uint8_t cmd = CMD_START_CONVERT;
	uint8_t addr = _addr_byte(h->address);
	h->transfer(addr, &cmd, sizeof(cmd), NULL, 0);
}


uint16_t ds1621_read_temperature(const i2c_handler_t *h) {
	uint8_t cmd = CMD_READ_TEMP;
	uint8_t ta[2];
	uint8_t addr = _addr_byte(h->address);
	h->transfer(addr, &cmd, sizeof(cmd), ta, sizeof(ta));
	uint16_t t = ((uint16_t)ta[0] << 1) + (ta[1] >> 7);
	return t;
}
