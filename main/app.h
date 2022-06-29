#pragma once

#include <esp_err.h>

#include "cjsonRpc.h"
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

typedef enum jsonrpc_method_e jsonrpc_method_t;
enum jsonrpc_method_e
{
    ping,
    status,
    add_aes_cbc_symkey,
};

jsonrpc_method_t get_method_from(const char* method);
RPCJson_t rs_rpc_parse_ping(const int id);
RPCJson_t rs_rpc_parse_status(const int id);
RPCJson_t rs_rpc_parse_add_aes_cbc_symkey(const int id, RPCJson_t* request);

void record(app_t* app, const char* fname);

esp_err_t first_init(app_t* app);
