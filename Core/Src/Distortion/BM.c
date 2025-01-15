/*
 * BM.c
 *
 *  Created on: Dec 8, 2024
 *      Author: Nezar
 */

static float out;
static float stage1;


float bmADAA(float x) {

    if (x > 0.4) {
        stage1 = 0.666667*x*x*x;
    } else if(x < -0.4) {
        stage1 = -1*0.666667*x*x*x;
    } else {
        stage1 = 0.041666*x*x*x*x;
    }

    stage1 *= 40;

    if (stage1 >= 0.4) {
        out = 0.666667*stage1*stage1*stage1;
    } else if(stage1 <= -0.4) {
        out = -1*0.666667*stage1*stage1*stage1; // slilcon diode hard clipping
    } else {
        out = 0.041666*stage1*stage1*stage1*stage1;
    }
    return out;
}

float bm(float in) {
    if (in > 0.4) {
        return 0.4;
    } else if (in < -0.4) {
        return -0.4;
    }

    stage1 = in * 40;

    if (stage1 > 0.4) {
		return 0.4;
	} else if (stage1 < -0.4) {
		return -0.4;
	} else {
		return stage1;
	}
}
