#include "convert_ps3.h"

//PS3 converter
//We will map 4 axis and 12 buttons onto 16 channels

static uint8_t throttle_lock = 0;
static uint16_t throttle = 1000;
static int16_t trim_roll = 0;
static int16_t trim_pitch = 0;



#define BTN_COUNT 17
static uint16_t btnPPM[BTN_COUNT];
static uint8_t btn[BTN_COUNT]; 

static uint16_t toggle2Stage(uint8_t ch) {
	if (btnPPM[ch]==1000) btnPPM[ch] = 2000;
	else btnPPM[ch] = 1000;

	return btnPPM[ch];
}

static uint16_t toggle3Stage(uint8_t ch) {
	if (btnPPM[ch]==1000) btnPPM[ch] = 1500;
	else if (btnPPM[ch]==1500) btnPPM[ch] = 2000;
	else btnPPM[ch] = 1000;

	return btnPPM[ch];
}

void convertInit() {
	uint8_t i;

	for (i=0;i<BTN_COUNT;i++) {
		btnPPM[i] = 1000;
		btn[i] = 0;
	}

}

int8_t convertButton(uint8_t n, uint8_t v, uint8_t *ch, uint16_t *ppm) { //this is triggered at 100Hz


	if (n>=BTN_COUNT) return 0;

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
		case 16: (*ch) = 4;  (*ppm) = toggle2Stage(*ch); return 1; break; //PS3
		case 0:  (*ch) = 5;  (*ppm) = toggle2Stage(*ch); return 1; break; //select
		case 3:  (*ch) = 6;  (*ppm) = toggle2Stage(*ch); return 1; break; //start

		case 1:  throttle_lock = 0; break; //l_thumb
		case 2:  throttle_lock = 1; break; //r_thumb

		case 4:  trim_pitch+=50; break; //up
		case 6:  trim_pitch-=50; break; //down		
		case 5:  trim_roll+=50; break; //right
		case 7:  trim_roll-=50; break; //left

		case 8:  throttle -= 15; break; //L2
		case 10: throttle += 15; break; //L1		
		case 9:  (*ch) = 12; (*ppm) = toggle2Stage(*ch); return 1; break; //R2
		case 11: (*ch) = 11; (*ppm) = toggle2Stage(*ch); return 1; break; //R1

		case 12: (*ch) = 7;  (*ppm) = toggle2Stage(*ch); return 1; break; //traingle
		case 13: (*ch) = 8;  (*ppm) = toggle2Stage(*ch); return 1; break; //circle
		case 14: (*ch) = 9;  (*ppm) = toggle2Stage(*ch); return 1; break; //x
		case 15: (*ch) = 10; (*ppm) = toggle2Stage(*ch); return 1; break; //square
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
			if (!throttle_lock) {
				throttle += map(v,32767,-32767, -10,10); //scalled down due to 100Hz
			} else {}; //using the pre-set throttle

			throttle = constrain(throttle,1000,2000);
			(*ppm) = throttle;
			(*ch) = 0;
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
