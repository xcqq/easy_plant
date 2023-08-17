#include <Arduino.h>
#include <BH1750.h>

#include "light.h"
#include "util.h"

BH1750 lightMeter;

int light_init()
{
    int ret;
    ret = lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE);
    DEBUG_PRINT("Light init: %d\n", ret);
    return 0;
}

float light_read()
{
    float ret;

    while (!lightMeter.measurementReady(true)) {
        yield();
    }
    ret = lightMeter.readLightLevel();
    DEBUG_PRINT("Light: %f\n", ret);
    return ret;
}
