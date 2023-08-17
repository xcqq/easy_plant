#ifndef CONFIG_H_
#define CONFIG_H_

#include <Arduino.h>
#include "util.h"

#define CONFIG_EEPROM_SIZE 256

int config_init();
int config_load_plant_info(struct config_plant_info *info);
int config_load_settings(struct config_settings *settings);
int config_save_plant_info(struct config_plant_info *info);
int config_save_settings(struct config_settings *settings);

#endif