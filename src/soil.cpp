#include <Arduino.h>
#include <driver/rmt.h>

#include "soil.h"
#include "util.h"

#define SOIL_POWER_PIN 3
#define SOIL_WATER_PIN GPIO_NUM_10
#define SOIL_WATER_CHANNEL RMT_CHANNEL_2
#define SOIL_WATER_CLK_DIV 1 // 80MHz / 1 = 80MHz
#define SOIL_WATER_TICK (80000000 / SOIL_WATER_CLK_DIV)
#define SOIL_WATER_TIMEOUT_US 100
#define SOIL_WATER_BUF_SIZE 4096
#define SOIL_WATER_AVG_COUNT 10

#define SOIL_FERTILIZE_PIN 2

#define STABLIZE_TIME 500

rmt_config_t rmt_rx_config = {
    .rmt_mode = RMT_MODE_RX,
    .channel = SOIL_WATER_CHANNEL,
    .gpio_num = SOIL_WATER_PIN,
    .clk_div = SOIL_WATER_CLK_DIV,
    .mem_block_num = 1,
    .rx_config =
        {
            .idle_threshold = SOIL_WATER_TIMEOUT_US,
            .filter_ticks_thresh = 0,
            .filter_en = false,
        },
};

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

    water = freq;
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

void soil_init()
{
    rmt_config(&rmt_rx_config);
    rmt_driver_install(rmt_rx_config.channel, SOIL_WATER_BUF_SIZE, 0);
    pinMode(SOIL_POWER_PIN, OUTPUT);
    soil_power(false);
    DEBUG_PRINT("Soil init done");
}

int soil_read_water(int *water)
{
    RingbufHandle_t rb = NULL;
    size_t rx_size = 0;
    rmt_item32_t *item = NULL;
    uint32_t pulse_count, pulse_duration_sum = 0, pulse_freq;
    int ret = 0, i;

    rmt_get_ringbuf_handle(SOIL_WATER_CHANNEL, &rb);
    if (rb) {
        soil_power(true);
        delay(STABLIZE_TIME);
        rmt_rx_start(SOIL_WATER_CHANNEL, true);
        soil_power(false);
        item = (rmt_item32_t *)xRingbufferReceive(rb, &rx_size, 10);
        if (item) {
            pulse_count = rx_size / sizeof(rmt_item32_t);
            for (i = 0; i < pulse_count && i < SOIL_WATER_AVG_COUNT; i++) {
                pulse_duration_sum += item[i].duration0 + item[i].duration1;
            }
            if (pulse_duration_sum == 0) {
                ERROR_PRINT("Pulse duration sum is 0");
                ret = -1;
            } else {
                pulse_freq = SOIL_WATER_TICK * i / pulse_duration_sum;
                DEBUG_PRINT("Pulse count: %d", pulse_count);
                DEBUG_PRINT("Pulse frequency: %d Hz", pulse_freq);
                *water = soil_water_convert(pulse_freq);
            }
            vRingbufferReturnItem(rb, (void *)item);
        } else {
            ERROR_PRINT("No pulses received");
            ret = -1;
        }
        rmt_rx_stop(SOIL_WATER_CHANNEL);
        soil_power(false);
    } else {
        ERROR_PRINT("Ringbuffer handle is NULL");
        ret = -1;
    }

    return ret;
}
