#ifndef DAC6551_H
#define DAC6551_H


#include "stm32f4xx_hal.h"
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
SPI_HandleTypeDef *hspi;//SPI Port
GPIO_TypeDef *cs_port;	//SYNC GPIO Port
uint16_t cs_pin;		//SYNC GPIO Pin
uint32_t vref_mv;		//Ref Voltage
} dac6551_t;


void dac6551_init(dac6551_t *d);


// Set raw 12 bit code (0 to 4095). Returns HAL status.
HAL_StatusTypeDef dac6551_write_code(dac6551_t *d, uint16_t code);


// Set by millivolts (0 - vref_mv). Returns HAL status.
HAL_StatusTypeDef dac6551_set_mv(dac6551_t *d, uint32_t mv);

#ifdef __cplusplus
}
#endif


#endif // DAC6551_H
