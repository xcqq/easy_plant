#include <Preferences.h>

#include "config.h"

#define CONFIG_NAMESPACE "config"
#define CONFIG_MAGIC "PLANT"

struct config_header {
    char magic[6];
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

int config_init()
{
    int ret = 0;
    char magic[6];
    Preferences pref;

    pref.begin(CONFIG_NAMESPACE, false);
    ret = pref.getString("magic", magic, 6);
    if(ret == 0){
        ERROR_PRINT("config magic not found, init config");
        pref.clear();
        pref.putString("magic", CONFIG_MAGIC);
    }
    else if(strcmp(magic, CONFIG_MAGIC) != 0){
        ERROR_PRINT("config magic not match, init config");
        pref.clear();
        pref.putString("magic", CONFIG_MAGIC);
    }
    else{
        DEBUG_PRINT("config magic match, load config");
    }
    pref.end();

    return ret;
}

int config_load_plant_info(struct config_plant_info *info)
{
    int ret = 0;
    Preferences pref;

    pref.begin(CONFIG_NAMESPACE, true);
    ret = pref.getBytes("plant_info", info, sizeof(struct config_plant_info));
    if(ret == 0){
        ERROR_PRINT("config plant info not found");
    }
    pref.end();

    return ret;
}

int config_load_settings(struct config_settings *settings)
{
    int ret = 0;
    Preferences pref;

    pref.begin(CONFIG_NAMESPACE, true);
    ret = pref.getBytes("settings", settings, sizeof(struct config_settings));
    if(ret == 0){
        ERROR_PRINT("config settings not found");
    }
    pref.end();

    return ret;
}

int config_save_plant_info(struct config_plant_info *info)
{
    int ret = 0;
    Preferences pref;

    pref.begin(CONFIG_NAMESPACE, false);
    ret = pref.putBytes("plant_info", info, sizeof(struct config_plant_info));
    if(ret == 0){
        ERROR_PRINT("config plant info save failed");
    }
    pref.end();

    return ret;
}

int config_save_settings(struct config_settings *settings)
{
    int ret = 0;
    Preferences pref;

    pref.begin(CONFIG_NAMESPACE, false);
    ret = pref.putBytes("settings", settings, sizeof(struct config_settings));
    if(ret == 0){
        ERROR_PRINT("config settings save failed");
    }
    pref.end();

    return ret;
}

