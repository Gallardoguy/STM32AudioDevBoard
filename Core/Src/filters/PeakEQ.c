/*
 * PeakEQ.c
 *
 *  Created on: Dec 8, 2024
 *      Author: Nezar
 */

#include <filters/PeakEQ.h>

void initEQ(PeakEQ *filt) {
//	filt->bw = 100;
	filt->w0 = 2 * PI_F * 1100 / 96590;
//	filt->alpha = sin(filt->w0) / (2 * 0.707);


	filt->x0 = 0;
	filt->x1 = 0;
	filt->x2 = 0;
	filt->y1 = 0;
	filt->y2 = 0;
}


float processEQ(PeakEQ *filt, float in){
	filt->x0 = in;
    filt->y0 = (filt->b0/filt->a0) * filt->x0 + (filt->b1/filt->a0) * filt->x1 + (filt->b2/filt->a0) * filt->x2 - (filt->a1/filt->a0) * filt->y1 - (filt->a2/filt->a0) * filt->y2;

    filt->x2 = filt->x1;
    filt->x1 = filt->x0;
    filt->y2 = filt->y1;
    filt->y1 = filt->y0;

    return filt->y0;
}

void setGain(PeakEQ *filt, float gain) {
	filt->A = pow(10, gain/40);

	filt->b0 = 1 + filt->alpha * filt->A;
	filt->b1 = -2 * cos(filt->w0);
	filt->b2 = 1 - filt->alpha * filt->A;
	filt->a0 = 1 + filt->alpha / filt->A;
	filt->a1 = -2 * cos(filt->w0);
	filt->a2 = 1 - filt->alpha / filt->A;
}


void setParams(PeakEQ *filt, float freq, float bw, float Q) {
//	filt->bw = bw;
	filt->w0 = 2 * PI_F * freq / 96590;
//	filt->alpha = sin(filt->w0) / (2 * Q);
	filt->alpha = sin(filt->w0)*sinh( log(2)/2 * bw * filt->w0/sin(filt->w0) );
}
