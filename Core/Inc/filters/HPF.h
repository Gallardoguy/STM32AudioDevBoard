/*
 * HPF.h
 *
 *  Created on: Dec 9, 2024
 *      Author: Nezar
 */

#ifndef INC_FILTERS_HPF_H_
#define INC_FILTERS_HPF_H_

#include "main.h"

typedef struct {
	float b0, b1, b2, a0, a1, a2;
	float alpha, w0;
	float x0, x1, x2;
	float y0, y1, y2;
} HPF;

void initHPF(HPF *filt);

float processHPF(HPF *filt, float in);

void setFreqHPF(HPF *filt, float freq);

#endif /* INC_FILTERS_HPF_H_ */
