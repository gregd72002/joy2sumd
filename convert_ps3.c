#include "convert_ps3.h"

//PS3 converter
//We will map 4 axis and 12 buttons onto 16 channels

#define THROTTLE_CH 0
static uint8_t throttle_lock = 0;
static uint16_t throttle = 1000;

static uint16_t toggle2Stage(uint16_t v) {
	if (v==1000) return 2000;
	else return 1000;
}

static uint16_t toggle3Stage(uint16_t v) {
	if (v==1000) return 1500;
	if (v==1500) return 2000;
	else return 1000;
}

int8_t convertButton(uint8_t n, uint8_t v, uint8_t *ch, uint16_t *ppm) { //this is triggered at 100Hz
	static uint16_t btnCh[12] = {1500};
	static uint8_t btn[16] = {0}; 

	if (n>=16) return 0;

	if (v==0) { //button up
		btn[n] = 0; 
		return 0;
	}

	if (v && btn[n]) { //button kept down 
		return 0;
	}

	//button down

	btn[n] = v;

	switch (n) {
		case 0: throttle_lock = !throttle_lock; break; //select
		case 1: break; //l_thumb
		case 2: break; //r_thumb
		case 3: (*ch) = 5; btnCh[*ch] = toggle2Stage(btnCh[*ch]);  (*ppm) = btnCh[*ch]; return 1; break; //start
		case 4: break; //up
		case 5: break; //right
		case 6: break; //down
		case 7: break; //left
		case 8: throttle -= 15; (*ch) = THROTTLE_CH; (*ppm) = throttle; return 1; break; //L2
		case 9: break; //R2
		case 10: throttle += 15; (*ch) = THROTTLE_CH; (*ppm) = throttle; return 1; break; //L1
		case 11: break; //R2
		case 12: (*ch) = 6; btnCh[*ch] = toggle2Stage(btnCh[*ch]);  (*ppm) = btnCh[*ch]; return 1; break; //traingle
		case 13: (*ch) = 7; btnCh[*ch] = toggle2Stage(btnCh[*ch]);  (*ppm) = btnCh[*ch]; return 1; break; //circle
		case 14: (*ch) = 8; btnCh[*ch] = toggle2Stage(btnCh[*ch]);  (*ppm) = btnCh[*ch]; return 1; break; //x
		case 15: (*ch) = 9; btnCh[*ch] = toggle2Stage(btnCh[*ch]);  (*ppm) = btnCh[*ch]; return 1; break; //square
		case 16: break; //PS3
	}


	return 0;
}

static int constrain(int x, int min, int max) {
	return x<min?min:(x>max?max:x);
}

static int16_t map(int16_t x, int16_t in_min, int16_t in_max, int16_t out_min, int16_t out_max) {
	int16_t ret = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	return constrain(ret,out_min,out_max);
}


int8_t convertAxis(uint8_t n, int16_t v, uint8_t *ch, uint16_t *ppm) { //this is triggered at 100Hz
	switch (n) {
		case 0: (*ppm) = map(v,-32767,32767, 1000,2000); (*ch) = 1; return 1; break; //L left-right; (32767, -32768)
		case 1: //L up-down; (32768, -32768)
			if (throttle_lock) return 0;
			throttle += map(v,32767,-32767, -10,10);
			throttle = constrain(throttle,1000,2000);
			(*ppm) = throttle;
			(*ch) = THROTTLE_CH;
			return 1;
		break;
		case 2: (*ppm) = map(v,-32767,32767, 1000,2000); (*ch) = 2; return 1; break; //R left-right; (32767, -32768)
		case 3: (*ppm) = map(v,32767,-32767, 1000,2000); (*ch) = 3; return 1; break; //R up-down; (32767, -32768)
		case 23: break; //accelerometer roll (4500;-4500); usable: 3000;-3000
		case 24: break; //accelerometer pitch (4500;-4500); usable: 3000;-3000
		case 25: break; //accelerometer z-axis (0:max; -4500);
	}

	return 0;
}
