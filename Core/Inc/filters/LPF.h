/*
 * tsTone.h
 *
 *  Created on: Dec 9, 2024
 *      Author: Nezar
 */

#ifndef INC_FILTERS_LPF_H_
#define INC_FILTERS_LPF_H_

#include "main.h"

typedef struct {
	float b0, b1, b2, a0, a1, a2;
	float alpha, w0;
	float x0, x1, x2;
	float y0, y1, y2;
} LPF;

void initLPF(LPF *filt);

float processLPF(LPF *filt, float in);

void setFreqLPF(LPF *filt, float freq);

#endif /* INC_FILTERS_LPF_H_ */
