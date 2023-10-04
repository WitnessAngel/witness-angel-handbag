#include "uart.h"

#include <driver/uart.h>
#include <esp_log.h>
#include <esp_system.h>
#include <sdkconfig.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAG "uart"

// Timeout threshold for UART = number of symbols (~10 tics) with unchanged
// state on receive pin
#define UART_READ_TOUT (3) // 3.5T * 8 = 28 ticks, TOUT=3 -> ~24..33 ticks

esp_err_t uart_init(uart_t* uart)
{
    uart->port_num = CONFIG_UART_PORT_NUM;
    uart->buf_size = CONFIG_UART_BUF_SIZE;
    uart->data = (uint8_t*)malloc(uart->buf_size);
    uart_config_t uart_config = {
        .baud_rate = CONFIG_UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
        .source_clk = UART_SCLK_APB,
    };

    // Set UART log level
    /* esp_log_level_set(TAG, ESP_LOG_INFO); */

    ESP_LOGI(TAG, "Start RS485 application test and configure UART.");

    // Install UART driver (we don't need an event queue here)
    // In this example we don't even use a buffer for sending data.
    ESP_ERROR_CHECK(
        uart_driver_install(uart->port_num, uart->buf_size * 2, 0, 0, NULL, 0));

    // Configure UART parameters
    ESP_ERROR_CHECK(uart_param_config(uart->port_num, &uart_config));

    ESP_LOGI(TAG, "UART set pins, mode and install driver.");

    // Set UART pins as per KConfig settings
    ESP_ERROR_CHECK(uart_set_pin(uart->port_num, CONFIG_UART_TXD,
                                 CONFIG_UART_RXD, CONFIG_UART_RTS,
                                 UART_PIN_NO_CHANGE));

    // Set RS485 half duplex mode
    ESP_ERROR_CHECK(uart_set_mode(uart->port_num, UART_MODE_RS485_HALF_DUPLEX));

    // Set read timeout of UART TOUT feature
    ESP_ERROR_CHECK(uart_set_rx_timeout(uart->port_num, UART_READ_TOUT));

    return (ESP_OK);
}
