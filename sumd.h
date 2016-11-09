#ifndef __SUMD__H__
#define __SUMD__H__

#include <stdint.h>
#include <inttypes.h>

void sumd_init();

void sumd_set(uint8_t ch, uint16_t v);

uint8_t *sumd_get(uint8_t *len);

#endif

