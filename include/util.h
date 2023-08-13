#ifndef UTIL_H_
#define UTIL_H_

#include <stdio.h>

#define VERION 0.1

#ifdef DEBUG
#define DEBUG_PRINT(x, ...) do { \
    Serial.printf("[%s][%s][%d] " x "\n", __FILENAME__, __func__, __LINE__, ##__VA_ARGS__); \
} while (0)
#else
#define DEBUG_PRINT(x, ...) do {} while (0)
#endif

#endif