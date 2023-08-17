#include <Preferences.h>

#include "config.h"

#define CONFIG_NAMESPACE "config"
#define CONFIG_MAGIC "PLANT"

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


