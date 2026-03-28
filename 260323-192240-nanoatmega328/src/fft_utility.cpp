#include "fft_utility.h"

// iterative FFT
void it_fft(uint8_t data[])
{
	int reals[SAMPLE_AMOUNT];
	int imags[SAMPLE_AMOUNT];

	for(uint8_t i = 0; i < SAMPLE_AMOUNT; i++){
		reals[i] = 0;
		imags[i] = 0;
	}

	for(uint8_t i = 0; i < SAMPLE_AMOUNT; i++){
		reals[bitReverse(i, 7)] = data[i] - 127;
	}

	// 0 to log2(SAMPLE_AMOUNT)
	for (uint8_t s = 1; s < 8; s++)
	{
		uint8_t m = 1 << s;
		uint8_t mHalf = m >> 1;
		for (uint8_t k = 0; k < SAMPLE_AMOUNT; k = k + m)
		{
			for (uint8_t j = 0; j < mHalf; j++)
			{
				uint8_t iangle = j * (SAMPLE_AMOUNT) / m;
				long sinVal = (int16_t)pgm_read_word_near(intsine128 + iangle);
				long cosVal = (int16_t)pgm_read_word_near(intcosine128 + iangle);

				long tReal, tImag, uReal, uImag;
				long xr = reals[k + j + mHalf];
            	long xi = imags[k + j + mHalf];

            	tReal = (cosVal * xr - sinVal * xi) >> 10;
            	tImag = (cosVal * xi + sinVal * xr) >> 10;

				uReal = reals[k + j];
				uImag = imags[k + j];

				reals[k + j] = (uReal + tReal);
				imags[k + j] = (uImag + tImag);
				reals[k + j + mHalf] = (uReal - tReal);
				imags[k + j + mHalf] = (uImag - tImag);
			}
		}
	}
	for (uint8_t i = 0; i < SAMPLE_AMOUNT; i++)
	{
		data[i] = (uint8_t)(calcMagnitude(reals[i], imags[i]) * spectrumMagScale);
	}
}

/*
 Modified FFT for byte array
 
 
 DISCLAIMER
 Linnes Lab code, firmware, and software is released under the
 MIT License (http://opensource.org/licenses/MIT).
 
 The MIT License (MIT)
 
 Copyright (c) 2020 Linnes Lab, Purdue University
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 
 */

//modified FFT from kickFFT, will overrides sample array

uint32_t calcMagnitude(signed long int x, signed long int y)
{
	return sqrt(x*x + y*y);
}

int averageData(const byte data[], int startIndex, int width)
{
    int sum = 0;
    for (int i = startIndex; i < (startIndex + width); i++)
    {
        sum += (int)data[i];
    }
    return (sum / width);
}

int bitReverse(int n, int bits) {
	int ans = 0;
	for(uint8_t i = 0; i < bits; i++)
	{
		ans = (ans << 1) | (n & 1);
		n >>= 1;
	}
	return ans;
}
