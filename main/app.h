#pragma once

#include <esp_err.h>

#include "i2sMicro.h"
#include "uart.h"
#include "vector.h"

#include <mbedtls/ctr_drbg.h>
#include <mbedtls/entropy.h>
#include <mbedtls/pk.h>
#include <mbedtls/rsa.h>

typedef struct app_s app_t;
struct app_s
{
    i2sMicro_t i2sMicro;
    uart_t uart;
};

typedef struct encrypted_uart_s encrypted_uart_t;
struct encrypted_uart_s
{
    mbedtls_pk_context pk;
    mbedtls_entropy_context entropy; /* entropy pool for seeding PRNG */
    mbedtls_ctr_drbg_context drbg;   /* pseudo-random generator */
    unsigned char buf_drbg[256];
    /* t_vector* vec; */
};

void record(app_t* app, const char* fname);

esp_err_t first_init(app_t* app);
