#pragma once

#include <esp_err.h>

#include "i2sMicro.h"

typedef struct app_s app_t;
struct app_s
{
    i2sMicro_t i2sMicro;
};

void record(app_t* app, const char* fname);
