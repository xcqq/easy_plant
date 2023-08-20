#include <Preferences.h>

#include "config.h"

#define CONFIG_NAMESPACE "config"
#define CONFIG_MAGIC "PLANT"

struct config_header config_header;
struct config_plant_info config_plant_info;
struct config_settings config_settings;

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

int config_load_header(struct config_header *header)
{
    int ret = 0;
    Preferences pref;

    pref.begin(CONFIG_NAMESPACE, false);
    ret = pref.getBytes("header", header, sizeof(struct config_header));
    if(ret == 0){
        ERROR_PRINT("config header not found");
        ERROR_PRINT("reset to default");
        pref.putBytes("header", &default_header, sizeof(struct config_header));
    }
    pref.end();

    return ret;
}

int config_load_plant_info(struct config_plant_info *info)
{
    int ret = 0;
    Preferences pref;

    pref.begin(CONFIG_NAMESPACE, false);
    ret = pref.getBytes("plant_info", info, sizeof(struct config_plant_info));
    if(ret == 0){
        ERROR_PRINT("config plant info not found");
        ERROR_PRINT("reset to default");
        pref.putBytes("plant_info", &default_plant_info, sizeof(struct config_plant_info));
    }
    pref.end();

    return ret;
}

int config_load_settings(struct config_settings *settings)
{
    int ret = 0;
    Preferences pref;

    pref.begin(CONFIG_NAMESPACE, false);
    ret = pref.getBytes("settings", settings, sizeof(struct config_settings));
    if(ret == 0){
        ERROR_PRINT("config settings not found");
        ERROR_PRINT("reset to default");
        pref.putBytes("settings", &default_settings, sizeof(struct config_settings));
    }
    pref.end();

    return ret;
}

int config_save_header(struct config_header *header)
{
    int ret = 0;
    Preferences pref;

    pref.begin(CONFIG_NAMESPACE, false);
    ret = pref.putBytes("header", header, sizeof(struct config_header));
    if(ret == 0){
        ERROR_PRINT("config header save failed");
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

