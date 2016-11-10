#include "sumd.h"

#define SUMD_HEADER_LEN 3
#define SUMD_CHANNEL_MAX 16
#define SUMD_CRC_LEN 2
#define SUMD_LEN (SUMD_HEADER_LEN+SUMD_CHANNEL_MAX*2+SUMD_CRC_LEN)

static uint8_t buf[SUMD_LEN]; 

static uint16_t sumd_crc16(uint16_t crc, uint8_t value)
{
    int i;
    crc ^= (uint16_t)value << 8;

    for (i = 0; i < 8; i++) {
	crc = (crc & 0x8000) ? (crc << 1) ^ 0x1021 : (crc << 1);
    }

    return crc;
}

void sumd_init() {
    uint8_t i;
    for (i=0;i<SUMD_CHANNEL_MAX;i++)
        sumd_set(i,1500);

    buf[0] = 0xA8;
    buf[1] = 0x01;
    buf[2] = 0x10;
}

void sumd_set(uint8_t ch, uint16_t v) {
    if (ch>=SUMD_CHANNEL_MAX) return;

    if (v>2000) v=2000;
    if (v<900) v=900;

    v *= 8;
    
    buf[SUMD_HEADER_LEN+2*ch] = v >> 8;
    buf[SUMD_HEADER_LEN+2*ch+1] = v & 0xFF;
}

uint8_t *sumd_get(uint8_t *len) {
    uint8_t i;
    uint16_t crc = 0;

    for (i=0;i<SUMD_LEN-SUMD_CRC_LEN;i++) 
	   crc = sumd_crc16(crc, buf[i]);

    buf[SUMD_LEN-2] = crc >> 8;
    buf[SUMD_LEN-1] = crc & 0xFF;

    (*len) = SUMD_LEN;

    return buf;
}

