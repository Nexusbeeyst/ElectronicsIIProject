#ifndef FFT_UTILITY_H
#define FFT_UTILITY_H

#include "globals.h"

void it_fft(uint8_t data[]);
uint32_t calcMagnitude(signed long int x, signed long int y);
int averageData(const byte data[], int startIndex, int width);
int bitReverse(int n, int bits);

#endif