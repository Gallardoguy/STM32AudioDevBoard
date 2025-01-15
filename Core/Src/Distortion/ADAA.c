/*
 * ADAA.c
 *
 *  Created on: Dec 8, 2024
 *      Author: Nezar
 */

#include <distortion/ADAA.h>
//#include <distortion/TS9.h>
//#include <distortion/HM.h>
//#include <distortion/BM.h>
//#include <distortion/fuzz.h>

static float x1 = 0;
static float x2 = 0;
static float x3 = 0;
static float f0 = 0;
static float f1 = 0;
static float f2 = 0;
static float f3 = 0;
static float out;
static float sigma = 0.000000001;
static float x_x1;
static float x1_x2;
static float x2_x3;
static float x1_x3;
static float x_x2;

float dist3ADAA(float x, uint8_t effect) {

    x_x1 = x-x1;
    x1_x2 = x1-x2;
    x2_x3 = x2-x3;
    x1_x3 = x1-x3;
    x_x2 = x-x2;


    if (effect == 1) {

        if(x_x1 < sigma){
            return ts( (x + x1) / 2);
        } else if(x1_x2 < sigma) {
            return ts( (x1 + x2) / 2);
        } else if(x2_x3 < sigma) {
            return ts( (x2 + x3) / 2);
        } else if(x1_x3 < sigma) {
            return ts( (x1 + x3) / 2);
        } else if(x_x2 < sigma) {
            return ts( (x + x2) / 2);
        }

        f0 = tsADAA(x);
//        f1 = tsADAA(x1);
//        f2 = tsADAA(x2);
//        f3 = tsADAA(x3);
    } else if(effect == 2) {

        if(x_x1 < sigma){
            return bm( (x + x1) / 2);
        } else if(x1_x2 < sigma) {
            return bm( (x1 + x2) / 2);
        } else if(x2_x3 < sigma) {
            return bm( (x2 + x3) / 2);
        } else if(x1_x3 < sigma) {
            return bm( (x1 + x3) / 2);
        } else if(x_x2 < sigma) {
            return bm( (x + x2) / 2);
        }

        f0 = bmADAA(x);
//        f1 = bmADAA(x1);
//        f2 = bmADAA(x2);
//        f3 = bmADAA(x3);
    } else if(effect == 3){
        if(x_x1 < sigma){
            return hm( (x + x1) / 2);
        } else if(x1_x2 < sigma) {
            return hm( (x1 + x2) / 2);
        } else if(x2_x3 < sigma) {
            return hm( (x2 + x3) / 2);
        } else if(x1_x3 < sigma) {
            return hm( (x1 + x3) / 2);
        } else if(x_x2 < sigma) {
            return hm( (x + x2) / 2);
        }

        f0 = hmADAA(x);
//        f1 = hmADAA(x1);
//        f2 = hmADAA(x2);
//        f3 = hmADAA(x3);
    } else {
    	if(x_x1 < sigma){
			return processFuzz( (x + x1) / 2);
		} else if(x1_x2 < sigma) {
			return processFuzz( (x1 + x2) / 2);
		} else if(x2_x3 < sigma) {
			return processFuzz( (x2 + x3) / 2);
		} else if(x1_x3 < sigma) {
			return processFuzz( (x1 + x3) / 2);
		} else if(x_x2 < sigma) {
			return processFuzz( (x + x2) / 2);
		}

		f0 = fuzzADAA(x);
//		f1 = fuzzADAA(x1);
//		f2 = fuzzADAA(x2);
//		f3 = fuzzADAA(x3);
    }

    out = 1/x1_x2 * ( 2/x_x2*((f0-f1)/(x_x1) - (f1-f2)/(x1_x2)) - 2/x1_x3*((f1-f2)/(x1_x2) - (f2-f3)/(x2_x3)));

    x3 = x2;
    x2 = x1;
    x1 = x;

    f3 = f2;
    f2 = f1;
    f1 = f0;

    return out;

}
