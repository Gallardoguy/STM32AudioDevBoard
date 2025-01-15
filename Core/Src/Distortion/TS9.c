/*
 * TS9.c
 *
 *  Created on: Dec 8, 2024
 *      Author: Nezar
 */

#include "main.h"
#include <distortion/TS9.h>


static float out;
static float absx;
static float xx;
static float numerator;

float tsADAA(float in) {//third anti-derivative of ts() function with respect to the input

	absx = fabs(in);
	xx = in*in;

	numerator = 6*(2*absx + xx + 1)*log(absx+1) - 2*(xx + 9)*absx - 9*xx;

	out = -1*numerator/12;

	return out;
}

//float tsADAA(float x) {
//    float abs_x = fabs(x); // Compute |x|
//    float x_squared = x * x; // Compute x^2
//    float log_term = logf(abs_x + 1); // Compute ln(|x| + 1)
//
//    // Break down the expression into smaller parts for efficiency
//    float term1 = 2 * abs_x + x_squared + 1; // 2|x| + x^2 + 1
//    float term2 = 2 * (x_squared + 9) * abs_x; // 2(x^2 + 9)|x|
//
//    // Compute the numerator
//    float numerator = -6 * (term1 * log_term - term2 - 9 * x_squared);
//
//    // Compute the final result
//    return numerator / 12.0;
//}

float ts(float in) {
    return in / (1 + fabs(in));
}
