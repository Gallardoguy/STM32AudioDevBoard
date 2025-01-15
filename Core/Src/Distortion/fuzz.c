/*
 * fuzz.c
 *
 *  Created on: Dec 10, 2024
 *      Author: Nezar
 */


#include "main.h"
#include <distortion/fuzz.h>


static float term1;
static float term2;
static float term3;
static float var;
static float coth;
static float out;

static float a_minus_1;
static float inv_a_minus_1;
static float exp_arg;
static float exp_val;
static float numerator;
static float denominator;

static float fuzz;

void setFuzz(float a) {
	fuzz = a;
}

float fuzzADAA(float in) {//third anti-derivative of fuzz() function with respect to the input

	a_minus_1 = fuzz - 1.0f;              // fuzz - 1
	inv_a_minus_1 = 1.0f / a_minus_1;  // Precompute 1/(fuzz - 1)
	exp_arg = fuzz * (1.0f - in) * inv_a_minus_1; // Simplified exponent argument
	exp_val = expf(exp_arg);           // Compute exponential
	numerator = -2.0f * a_minus_1 * a_minus_1 * a_minus_1 * exp_val; // Cube of (fuzz - 1)
	denominator = fuzz * fuzz * fuzz;           // Cube of fuzz

	return numerator / denominator;
}

float processFuzz(float in) {
	term1 = 1+expf((2*fuzz)/(fuzz-1));
	term2 = 2*expf((fuzz*(1-in))/(fuzz-1));
	term3 = term1-term2;
	var = fuzz/(fuzz-1);
	coth = coshf(var)/sinhf(var) - 1;
	out = term3*coth*-1/2;

	return out;
}
