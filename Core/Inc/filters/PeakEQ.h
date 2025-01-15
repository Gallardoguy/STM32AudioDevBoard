/*
 * PeakEq.h
 *
 *  Created on: Dec 8, 2024
 *      Author: Nezar
 */

#ifndef INC_PEAKEQ_H_
#define INC_PEAKEQ_H_

#include "main.h"

//All filters follow the exact same difference equation. The coefficents are the only difference

#define PI_F 3.1415927410125732421875f

typedef struct {
	float b0, b1, b2, a0, a1, a2;
	float fs, f0, dbgain;
	float A, alpha, w0;
	float x0, x1, x2;
	float y0, y1, y2;
} PeakEQ;

void initEQ(PeakEQ *filt);

float processEQ(PeakEQ *filt, float in);

void setGain(PeakEQ *filt, float gain);

void setParams(PeakEQ *filt, float freq, float bw, float Q);

#endif /* INC_PEAKEQ_H_ */
