#include <Arduino.h>
#include <driver/gpio.h>

#include "soil.h"
#include "util.h"

#define SOIL_POWER_PIN 3
#define SOIL_WATER_PIN 10
#define SOIL_FERTILIZE_PIN 2
#define SOIL_WATER_AVG_COUNT 100000

#define STABLIZE_TIME 500

static void soil_power(bool on)
{
    if (on) {
        digitalWrite(SOIL_POWER_PIN, HIGH);
    } else {
        digitalWrite(SOIL_POWER_PIN, LOW);
    }
}

// convert freq to water level(0-100%)
static int soil_water_convert(uint32_t freq)
{
    int water;

    // TODO: convert freq to water level
    water = freq / 1000;
    DEBUG_PRINT("Soil freq: %d -> water: %d", freq, water);
    return water;
}

// convert volt to water level(0-100%)
static int soil_fertilize_convert(int volt)
{
    int fertilize;

    fertilize = volt;
    DEBUG_PRINT("Soil volt: %d -> fertilize: %d", volt, fertilize);
    return fertilize;
}

// start by register pulse isr and initialize count to 0
// end by count == SOIL_WATER_AVG_COUNT
uint64_t start_time = 0;
uint64_t end_time = 0;
uint64_t count = 0;
uint64_t freq_khz = 0;
void IRAM_ATTR pulse_isr()
{
    if (count == 0) {
        start_time = micros();
    }
    if (count >= SOIL_WATER_AVG_COUNT) {
        end_time = micros();
        detachInterrupt(SOIL_WATER_PIN);
        freq_khz = 1000 * SOIL_WATER_AVG_COUNT / (end_time - start_time);
        DEBUG_PRINT("start_time: %llu, end_time: %llu diff:%llu", start_time, end_time,
                    end_time - start_time);
        DEBUG_PRINT("freq: %llukhz", freq_khz);
    } else {
        count++;
    }
}
void soil_init()
{
    pinMode(SOIL_POWER_PIN, OUTPUT);
    soil_power(false);
    pinMode(SOIL_WATER_PIN, INPUT);
    DEBUG_PRINT("Soil init done");
}

int soil_read_water(int *water)
{
    // uint32_t pulse_count, pulse_duration_sum = 0, pulse_freq;
    int ret = 0;

    count = 0;
    freq_khz = 0;
    soil_power(true);
    delay(STABLIZE_TIME);
    attachInterrupt(SOIL_WATER_PIN, pulse_isr, RISING);
    // statically delay 1s to wait for count to be 100000
    // freq is around 400k, it should be enough to wait for 100000
    // TODO: dynamically wait for count to be 100000
    delay(500);
    soil_power(false);
    if (freq_khz != 0) {
        *water = soil_water_convert(freq_khz);
        DEBUG_PRINT("Soil water: %d", *water);
    } else {
        DEBUG_PRINT("Failed to read soil water");
        ret = -1;
    }

    return ret;
}
