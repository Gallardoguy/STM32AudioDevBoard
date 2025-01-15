/*
 * BPF.h
 *
 *  Created on: Dec 9, 2024
 *      Author: Nezar
 */

#ifndef INC_FILTERS_BPF_H_
#define INC_FILTERS_BPF_H_

#include "main.h"

#define PI_F 3.1415927410125732421875f

typedef struct {
	float b0, b1, b2, a0, a1, a2;
	float alpha, w0;
	float x0, x1, x2;
	float y0, y1, y2;
} BPF;

void initBPF(BPF *filt);

float processBPF(BPF *filt, float in);

void setFreqBPF(BPF *filt, float freq, float BW);

#endif /* INC_FILTERS_BPF_H_ */
