#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <FastLED.h>
#include "fl/leds.h"
#include "fl/xymap.h"
#include "fft_utility.h"

#define AUDIO_INPUT A0
#define NOISE_MONITOR_ENABLE 7
#define BUZZER_OUTPUT 9
#define LED_PIN 2
#define NUM_LEDS 256
#define LED_HEIGHT 8
#define SAMPLE_AMOUNT 128
#define BUFFER_ARRAY_SIZE 8

const float inputGain = 1; // NOTE: changing this causes reduced performance, likely due to added casting
const float noiseLevelDisplayScale = 4; // scales RMS value when displaying
const float spectrumMagScale = 1.3;
const float samplingRate = 9600; // determined by experimentation
const uint8_t brightness = 12; // <= 20 will give keep current load under 1 amp
const uint8_t noiseThreshold = 48; // RMS value that must be reached for a noise event to be considered loud enough
const float buzzerDuration = 500; // ms
const uint16_t buzzerTone = 4000;

// debugging
const bool frequencyTest = false;
const float testFrequency = 200;

const CRGB visualizerColor[LED_HEIGHT] = {CRGB::DarkGreen, CRGB::ForestGreen, CRGB::Green, CRGB::YellowGreen, CRGB::Yellow, CRGB::Yellow, CRGB::Red, CRGB::Red};

// stored in flash to save memory
const int intsine128[128] PROGMEM = {
0,
-49,-98,-146,-195,-242,-290,-336,-382,-427,-471,
-514,-555,-595,-634,-671,-707,-740,-773,-803,-831,
-857,-881,-903,-923,-941,-956,-970,-980,-989,-995,
-998,-1000,-998,-995,-989,-980,-970,-956,-941,-923,
-903,-881,-857,-831,-803,-773,-740,-707,-671,-634,
-595,-555,-514,-471,-427,-382,-336,-290,-242,-195,
-146,-98,-49,0,49,98,146,195,242,290,
336,382,427,471,514,555,595,634,671,707,
740,773,803,831,857,881,903,923,941,956,
970,980,989,995,998,1000,998,995,989,980,
970,956,941,923,903,881,857,831,803,773,
740,707,671,634,595,555,514,471,427,382,
336,290,242,195,146,98,49
};

const int intcosine128[128] PROGMEM = {
1000,
998,995,989,980,970,956,941,923,903,881,
857,831,803,773,740,707,671,634,595,555,
514,471,427,382,336,290,242,195,146,98,
49,0,-49,-98,-146,-195,-242,-290,-336,-382,
-427,-471,-514,-555,-595,-634,-671,-707,-740,-773,
-803,-831,-857,-881,-903,-923,-941,-956,-970,-980,
-989,-995,-998,-1000,-998,-995,-989,-980,-970,-956,
-941,-923,-903,-881,-857,-831,-803,-773,-740,-707,
-671,-634,-595,-555,-514,-471,-427,-382,-336,-290,
-242,-195,-146,-98,-49,0,49,98,146,195,
242,290,336,382,427,471,514,555,595,634,
671,707,740,773,803,831,857,881,903,923,
941,956,970,980,989,995,998
};

#endif