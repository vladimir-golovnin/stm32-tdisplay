#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>

#include "ds1621.h"


#define BUS I2C1
#define ADDRESS 0

i2c_handler_t handler;

void transfer(uint8_t address, uint8_t *wb, size_t wsize, uint8_t *rb, size_t rsize){
	i2c_transfer7(BUS, address, wb, wsize, rb, rsize);
}


int tsensor_init() {
        rcc_periph_clock_enable(RCC_I2C1);
        gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_10_MHZ, GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN, GPIO6 | GPIO7);

// The following is the required sequence in master mode.
// Program the peripheral input clock in I2C_CR2 Register in order to generate correct timings
        i2c_set_standard_mode(BUS);
        i2c_set_clock_frequency(BUS, I2C_CR2_FREQ_8MHZ);
// Configure the clock control registers
        i2c_set_ccr(BUS, 40);
// Configure the rise time register
        i2c_set_trise(BUS, 9);
// Program the I2C_CR1 register to enable the peripheral
        i2c_peripheral_enable(BUS);

	handler.address = ADDRESS;
	handler.transfer = transfer;
	ds1621_set_config(&handler, DS1621_CFG_CONT);
	ds1621_start_convert(&handler);
	return 0;
}


#define CORRECTION 5

int tsensor_get_temp() {
	uint16_t tv = ds1621_read_temperature(&handler);
	int t = (tv >> 1) - CORRECTION;
	return (tv & 1) ? (t + 1) : t; 
}
