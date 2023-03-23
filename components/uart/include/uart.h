#pragma once

#include <esp_err.h>

typedef struct uart_s uart_t;
struct uart_s
{
    int port_num;
    uint buf_size;
    uint8_t* data;
};

esp_err_t uart_init(uart_t* uart);

// uart_destroy() free data;
