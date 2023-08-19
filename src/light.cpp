#include <Arduino.h>
#include <BH1750.h>

#include "light.h"
#include "util.h"

BH1750 lightMeter;

int light_init()
{
    int ret;

    Wire.begin(4, 5);
    ret = lightMeter.begin(BH1750::CONTINUOUS_LOW_RES_MODE);
    DEBUG_PRINT("Light init: %d\n", ret);

    return ret;
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

int light_power_off()
{
    int ret;

    ret = lightMeter.begin(BH1750::UNCONFIGURED);
    DEBUG_PRINT("Light power off: %d\n", ret);

    return 0;
}
