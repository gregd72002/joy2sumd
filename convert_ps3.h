#ifndef __CONVERT_PS3_H_
#define __CONVERT_PS3_H_

#include <stdint.h>
#include <inttypes.h>

void convertInit();

int8_t convertButton(uint8_t n, uint8_t v, uint8_t *ch, uint16_t *ppm);

int8_t convertAxis(uint8_t n, int16_t v, uint8_t *ch, uint16_t *ppm);

#endif

