#include "globals.h"

CRGB leds[NUM_LEDS];
uint8_t samples[SAMPLE_AMOUNT];
uint8_t RMSBuffer[BUFFER_ARRAY_SIZE] = {0}; // array of RMS values

void addressLED(int x, int y, CRGB color);
uint8_t calculateRMS(uint8_t data[]);
bool isNoiseEvent(uint8_t val);

float executionTime = 0.0;
float lastTime = 0.0;
uint8_t lastBufferIndex = 0;
bool isBuzzerActive = false;
bool isNoiseSensing;
float buzzerTimer = 0;

void setup() {
    pinMode(BUZZER_OUTPUT, OUTPUT);
    pinMode(NOISE_MONITOR_ENABLE, INPUT);
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(brightness);
    Serial.begin(9600);
}

void loop() {
    lastTime = millis();

    isNoiseSensing = digitalRead(NOISE_MONITOR_ENABLE);

    if (isBuzzerActive)
    {
        tone(BUZZER_OUTPUT, buzzerTone);
    }

    for (int i = 0; i < SAMPLE_AMOUNT; i++)
    {
        if (frequencyTest)
        {
            float samplePoint = 80 * sin(2 * 3.1415 * (i/samplingRate) * testFrequency) + 128;
            samples[i] = samplePoint;
        }
        else if (!isBuzzerActive)
        {
            // 0-1023 -> 0-255 via bit shift 
            samples[i] = analogRead(AUDIO_INPUT) >> 2;
        }
        else 
        {
            samples[i] = 127; // reset to zero val;
        }
    }

    if (isNoiseSensing){
        RMSBuffer[lastBufferIndex] = calculateRMS(samples);
        lastBufferIndex++;

        if (lastBufferIndex > BUFFER_ARRAY_SIZE - 1)
        {
            lastBufferIndex = 0;
        } 

        if (isNoiseEvent(RMSBuffer[lastBufferIndex]))
        {
            isBuzzerActive = true;
            buzzerTimer = buzzerDuration;
        }
    } else {
        it_fft(samples);
    }

    // Serial.println("Timing fft:");
    // lastTime = millis();    

    if (!isBuzzerActive) // dont update the LEDs while buzzing
    {
        FastLED.clear();
    }

    // if in noise sensing mode, display horizontally the noise value
    int width = NUM_LEDS / LED_HEIGHT;
    uint8_t lineWidth = 0;
    if (isNoiseSensing)
    {
        // map 0-255 to the width of the LED, one dimensional representation
        lineWidth = (RMSBuffer[lastBufferIndex]*noiseLevelDisplayScale) / (256 / width);
    }
    for (int x = 0; x < width; x++)
    {
        uint8_t lineHeight = 0;
        if (isNoiseSensing)
        {
            lineHeight = (x < lineWidth) ? LED_HEIGHT : 0;
            uint8_t colorIndex = lineWidth / 4; // map width (32) to color index so that the color is horizontal
            for (int drawHeight = 0; drawHeight < lineHeight; drawHeight++)
            {
                addressLED((width - 1) - x, drawHeight, visualizerColor[colorIndex]);
            }
        }
        else
        {
            // map 0-255 to 0-7;
            int y = averageData(samples, x, SAMPLE_AMOUNT / width) / (255/LED_HEIGHT);
            lineHeight = y;
            for (int drawHeight = 0; drawHeight < lineHeight; drawHeight++)
            {
                addressLED((width - 1) - x, drawHeight, visualizerColor[drawHeight]);
            }
        }
    }   

    if (!isBuzzerActive) // dont update the LEDs while buzzing
    {
        FastLED.show();
    }
    
    if (frequencyTest)
    {
        Serial.println("Freq(Hz),Magnitude");
        for(uint16_t i = 0; i < SAMPLE_AMOUNT; i++)
        {
            Serial.print(i*samplingRate/SAMPLE_AMOUNT/2);
            Serial.print(",");
            Serial.print(samples[i]);
            Serial.println();
        }
        delay(1000);
    }

    if (isBuzzerActive)
    {
        buzzerTimer -= (millis() - lastTime);
        if (buzzerTimer <= 0)
        {
            isBuzzerActive = false;
            noTone(BUZZER_OUTPUT);
        }
    }
}

void addressLED(int x, int y, CRGB color)
{
    int index = 0;
    // account for the fact that LEDs are indexed in alternating directions
    if ((x + 1) % 2 == 0)
    {
        index = ((x * LED_HEIGHT)) + (LED_HEIGHT - 1) - y;
    }
    else
    {
        index = ((x * LED_HEIGHT)) + y;
    }
    leds[index] = color;
}

uint8_t calculateRMS(uint8_t data[])
{
    uint32_t sum = 0; // sum of n^2 for all samples can be up to 128^3
    for (int i = 0; i < SAMPLE_AMOUNT; i++)
    {
        int16_t val = data[i] - 127;
        sum += val*val;
    }
    sum /= SAMPLE_AMOUNT;
    return sqrt(sum);
}

// if it's greater than 1/2 RMS samples and above the threshold, it is considered a noise event 
bool isNoiseEvent(uint8_t val)
{
    uint8_t lessers = 0;

    for (uint8_t i = 0; i < BUFFER_ARRAY_SIZE; i++)
    {
        if (RMSBuffer[i] < val){
            lessers++;
        }
    }

    return (lessers >= BUFFER_ARRAY_SIZE/2) && (val > noiseThreshold);
}