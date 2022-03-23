#pragma once

#include <esp_err.h>

#include "i2sMicro.h"
#include "uart.h"

typedef struct app_s app_t;
struct app_s
{
    i2sMicro_t i2sMicro;
    uart_t uart;
};

void record(app_t* app, const char* fname);

esp_err_t first_init(app_t* app);
