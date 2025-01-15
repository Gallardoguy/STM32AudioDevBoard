/*
 * BPF.c
 *
 *  Created on: Dec 9, 2024
 *      Author: Nezar
 */

#include <filters/BPF.h>

void initBPF(BPF *filt) {
	filt->w0 = 2 * PI_F * 1100 / 96590;
	filt->alpha = sinf(filt->w0)*sinhf( logf(2)/2 * 100 * filt->w0/sinf(filt->w0) );


	filt->x0 = 0;
	filt->x1 = 0;
	filt->x2 = 0;
	filt->y1 = 0;
	filt->y2 = 0;
}


void setFreqBPF(BPF *filt, float freq, float BW) {
	filt->w0 = 2 * PI_F * freq / 96590;
	filt->alpha = sinf(filt->w0)*sinhf( logf(2)/2 * BW * filt->w0/sinf(filt->w0) );

	filt->b0 =  filt->alpha;
	filt->b1 =   0;
	filt->b2 =  -1*filt->alpha;
	filt->a0 =   1 + filt->alpha;
	filt->a1 =  -2*cosf(filt->w0);
	filt->a2 =   1 - filt->alpha;

}

float processBPF(BPF *filt, float in) {
	filt->x0 = in;
	filt->y0 = (filt->b0/filt->a0) * filt->x0 + (filt->b1/filt->a0) * filt->x1 + (filt->b2/filt->a0) * filt->x2 - (filt->a1/filt->a0) * filt->y1 - (filt->a2/filt->a0) * filt->y2;

	filt->x2 = filt->x1;
	filt->x1 = filt->x0;
	filt->y2 = filt->y1;
	filt->y1 = filt->y0;

	return filt->y0;
}
