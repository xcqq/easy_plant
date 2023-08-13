#include "config.hpp"
#include <EEPROM.h>

#define HEADER_MAGIC "PLANT"

int config_init()
{
    int ret = 0;
    char header[6];

    EEPROM.begin(CONFIG_EEPROM_SIZE);
    EEPROM.get(0, header);
    DEBUG_PRINT("header: %s", header);
    if (memcmp(header, HEADER_MAGIC, 5) != 0) {
        EEPROM.put(0, HEADER_MAGIC);
        ret = EEPROM.commit();
        if (ret != true) ret = -ENODEV;
        ERROR_PRINT("EEPROM init failed ret: %d", ret);
    }

    return ret;
}


