#include "SHT2x.h"
#include <Arduino.h>
#include <Wire.h>

#include "air.h"
#include "util.h"

#define SHT2x_ADDR 0x40

SHT2x sht20;
int air_init()
{
    int ret;

    sht20.begin(4, 5);
    if (!sht20.getStatus()) {
        ERROR_PRINT("Failed to init SHT20");
        ret = -1;
    } else {
        ret = sht20.getFirmwareVersion();
        DEBUG_PRINT("SHT20 firmware version: %d", ret);
        ret = 0;
    }

    return ret;
}

int air_get_temp_humi(float *temp, float *humi)
{
    float value;
    int ret;

    ret = sht20.isConnected();
    if (!ret) return -1;
    // need a short delay here?
    if (sht20.read()) {
        value = sht20.getTemperature();
        *temp = value;
        value = sht20.getHumidity();
        *humi = value;
        DEBUG_PRINT("Temperature: %.1f, Humidity: %.1f", *temp, *humi);
    } else {
        ERROR_PRINT("Failed to read SHT20");
    }

    return ret;
}
