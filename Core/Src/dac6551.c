#include "dac6551.h"


static inline void cs_low(dac6551_t *d) { HAL_GPIO_WritePin(d->cs_port, d->cs_pin, GPIO_PIN_RESET); }
static inline void cs_high(dac6551_t *d) { HAL_GPIO_WritePin(d->cs_port, d->cs_pin, GPIO_PIN_SET); }


void dac6551_init(dac6551_t *d) {
	cs_high(d);
	dac6551_write_code(d, 0);
}


HAL_StatusTypeDef dac6551_write_code(dac6551_t *d, uint16_t code)
{
	//Clamp to 12-bit and make last 4-bits 0
	code &= 0x0FFF;
	uint32_t shift = code << 4;

	uint8_t frame[3];

	//Shift buffer to match frame layout
	frame[0] = (shift >>16) & 0xFF;		//[23:16] = 0 (don't care + PD1:PD0)
	frame[1] = (shift >> 8)  & 0xFF;	//[15:8] = Data (D11-D4)
	frame[2] =  shift & 0xFF;			//[7:4] = Data (D3-D0); [3:0] = 0 (padding)

	//Transmit to dac6551
	cs_low(d);
	HAL_StatusTypeDef status = HAL_SPI_Transmit(d->hspi, frame, 3, HAL_MAX_DELAY);
	cs_high(d);

	return status;
}


HAL_StatusTypeDef dac6551_set_mv(dac6551_t *d, uint32_t mv)
{
	uint16_t code = (mv * 4096) / d->vref_mv;

	return dac6551_write_code(d, code);

}
