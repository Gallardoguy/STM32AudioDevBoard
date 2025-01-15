/*
 * HM.c
 *
 *  Created on: Dec 8, 2024
 *      Author: Nezar
 */

#include "main.h"

static float out;
static float stage1;
static float out;
static float absx;
static float xx;
static float numerator;

float hmADAA(float in) {//third anti-derivative of hm() function with respect to the input
    absx = fabs(in);
    xx = in*in;

    numerator = 6*(2*absx + xx + 1)*log(absx+1) - 2*(xx + 3)*absx - 9*xx;

    stage1 = -1*numerator/12;

    stage1 *= 40;

    if (stage1 >= 0.17) {
        out = 0.000002833*stage1*stage1*stage1;
    } else if(stage1 <= -0.17) {
        out = -1*0.000002833*stage1*stage1*stage1; // germanium diode hard clipping approximation
    } else {
        out = 0.041666*stage1*stage1*stage1*stage1;
    }
    return out;
}

float hm(float in) {

	stage1 = in / (1 + fabs(in));

	stage1 *= 40;

    if (stage1 > 0.17) {
        return 0.17;
    } else if (stage1 < -0.17) {
        return -0.17;
    } else {
        return stage1;
    }
}
