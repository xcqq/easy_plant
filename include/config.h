#ifndef CONFIG_H_
#define CONFIG_H_

#include <Arduino.h>
#include "util.h"

struct config_header {
    uint8_t version; // version update need to clear all config
    uint8_t sub_version; // sub version update can keep config
    uint8_t reserved[64];
    // TODO: checksum needed?
};

#define CONFIG_PLANT_NAME_LEN 32
struct config_plant_info {
    char name[CONFIG_PLANT_NAME_LEN];
    uint16_t water_min;
    uint16_t water_max;
    uint16_t fertilize_min;
    uint16_t fertilize_max;
    uint16_t temp_min;
    uint16_t temp_max;
    uint16_t humi_min;
    uint16_t humi_max;
    uint16_t light_min;
    uint16_t light_max;
    uint16_t light_total_min;
    uint16_t light_total_max;
    uint8_t reserved[64];
};
struct config_settings {
    uint16_t capture_interval ; // in seconds
    uint16_t refresh_interval ; // in seconds
    uint16_t upload_interval ; // in seconds
    uint8_t reserved[64];
};

const struct config_header default_header = {
    .version = VERION,
    .sub_version = SUB_VERSION,
    .reserved = {0},
};

const struct config_plant_info default_plant_info = {
    .water_min = 0,
    .water_max = 100,
    .fertilize_min = 0,
    .fertilize_max = 100,
    .temp_min = 0,
    .temp_max = 30,
    .humi_min = 0,
    .humi_max = 100,
    .light_min = 0,
    .light_max = 100,
    .light_total_min = 0,
    .light_total_max = 100,
    .reserved = {0},
};

const struct config_settings default_settings = {
    .capture_interval = 60,
    .refresh_interval = 60,
    .upload_interval = 60,
    .reserved = {0},
};

extern struct config_header config_header;
extern struct config_plant_info config_plant_info;
extern struct config_settings config_settings;

int config_init();
int config_load_header(struct config_header *header);
int config_load_plant_info(struct config_plant_info *info);
int config_load_settings(struct config_settings *settings);
int config_save_header(struct config_header *header);
int config_save_plant_info(struct config_plant_info *info);
int config_save_settings(struct config_settings *settings);

#endif