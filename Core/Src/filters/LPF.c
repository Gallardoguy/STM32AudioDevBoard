/*
 * tsTone.c
 *
 *  Created on: Dec 9, 2024
 *      Author: Nezar
 */
#include <filters/LPF.h>

#define PI_F 3.1415927410125732421875f
#define Q 0.707 //resonance .707 = no resonance

void initLPF(LPF *filt) {
	filt->w0 = 2 * PI_F * 1100 / 96590;
	filt->alpha = sinf(filt->w0) / (2 * Q);


	filt->x0 = 0;
	filt->x1 = 0;
	filt->x2 = 0;
	filt->y1 = 0;
	filt->y2 = 0;
}


void setFreqLPF(LPF *filt, float freq) {
	filt->w0 = 2 * PI_F * freq / 96590;
	filt->alpha = sinf(filt->w0) / (filt->w0 * Q);

	filt->b0 =  (1 - cosf(filt->w0))/2;
	filt->b1 =   1 - cosf(filt->w0);
	filt->b2 =  (1 - cosf(filt->w0))/2;
	filt->a0 =   1 + filt->alpha;
	filt->a1 =  -2*cosf(filt->w0);
	filt->a2 =   1 - filt->alpha;

}

float processLPF(LPF *filt, float in) {
	filt->x0 = in;
	filt->y0 = (filt->b0/filt->a0) * filt->x0 + (filt->b1/filt->a0) * filt->x1 + (filt->b2/filt->a0) * filt->x2 - (filt->a1/filt->a0) * filt->y1 - (filt->a2/filt->a0) * filt->y2;

	filt->x2 = filt->x1;
	filt->x1 = filt->x0;
	filt->y2 = filt->y1;
	filt->y1 = filt->y0;

	return filt->y0;
}
