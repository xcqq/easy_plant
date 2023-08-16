#include <Arduino.h>
#include <SHT2x.h>
#include <Wire.h>

#include "air.h"
#include "util.h"

#define SHT2x_ADDR 0x40

SHT2x SHT20 = SHT2x();
int air_init()
{
    int ret;

    SHT20.begin(4, 5);
    if (!SHT20.isConnected()) {
        ERROR_PRINT("Failed to init SHT20");
        ret = -1;
    } else {
        ret = SHT20.getFirmwareVersion();
        DEBUG_PRINT("SHT20 firmware version: %d", ret);
        ret = 0;
    }

    return ret;
}

int air_get_temp(int *temp)
{
    int ret;

    SHT20.read();
    ret = SHT20.readTemperature();
    if (ret < 0)
        ERROR_PRINT("Failed to read temperature, ret=%d", ret);
    else {
        *temp = ret;
        ret = 0;
        DEBUG_PRINT("Temperature: %d", ret);
    }

    return ret;
}

int air_get_humi(int *humi)
{
    int ret;

    SHT20.read();
    ret = SHT20.readHumidity();
    if (ret < 0)
        ERROR_PRINT("Failed to read humidity, ret=%d", ret);
    else {
        *humi = ret;
        ret = 0;
        DEBUG_PRINT("Humidity: %d", ret);
    }

    return ret;
}
