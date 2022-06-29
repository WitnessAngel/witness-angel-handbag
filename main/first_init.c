#include "app.h"
#include "cjsonRpc.h"
#include "uart.h"
#include "vector.h"

#include <cJSON.h>
#include <driver/uart.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>
#include <stdlib.h>

#include <mbedtls/ctr_drbg.h>
#include <mbedtls/entropy.h>
#include <mbedtls/pk.h>
#include <mbedtls/rsa.h>

#define TAG "first_init"

/* CONFIG_UART_PORT_NUM = 2
   CONFIG_UART_BAUD_RATE = 115200
   CONFIG_UART_RXD = 3
   CONFIG_UART_TXD = 1
   CONFIG_UART_RTS = 22 */

static void rs_send(const int port, const char* str, size_t size)
{
    if (uart_write_bytes(port, str, size) != size)
    {
        ESP_LOGE(TAG, "Send data critical failure.");
        // add your code to handle sending failure here
        abort();
    }
}

static void rs_rpc_parse_string(uart_t* uart, t_vector* input)
{
    RPCJson_t request;
    RPCJson_t response;

    if (!rpc_json_parse(&request, (char*)input->data))
    {
        rs_send(uart->port_num, "\nError: 'rpc json parse fail'",
                strlen("\nError: 'rpc json parse fail'"));
        return;
    }
    else if (request.type == RPC_JSON_REQUEST)
    {
        int id;
        jsonrpc_method_t method =
            get_method_from(rpc_json_get_method(&request));
        rpc_json_get_id(&request, &id);

        switch (method)
        {
            case ping:
                response = rs_rpc_parse_ping(id);
                break;
            case status:
                response = rs_rpc_parse_status(id);
                break;
            case add_aes_cbc_symkey:
                response = rs_rpc_parse_add_aes_cbc_symkey(id, &request);
                break;
            default:
                rpc_json_create_error(&response, id, 101,
                                      "rpc_json_parse error");
                break;
        }
    }
    char* json_string = cJSON_PrintUnformatted(response.rpc_json_obj);
    rs_send(uart->port_num, json_string, strlen(json_string));

    /* snprintf(debug, 100, "\nInfo:\nid = %d", id); */
    /* rs_send(uart->port_num, debug, strlen(debug)); */

    /* if (!rpc_json_create_response(&response, id, 42))
    {
        ESP_LOGE(TAG, "rpc json create response fail");
        rs_send(uart->port_num, "\nError:\nrpc json create response fail",
                strlen("\nError:\nrpc json create response fail"));
        return;
        } */
}

// uart_task with hardware flow control on UART
static void uart_task(void* arg)
{
    uart_t* uart = (uart_t*)arg;

    ESP_LOGI(TAG, "UART start receive loop.\r\n");

    while (1)
    {
        // Read data from UART
        int len =
            uart_read_bytes(uart->port_num, uart->data, uart->buf_size,
                            1000 / portTICK_PERIOD_MS); // PACKET_READ_TICS

        // Write data back to UART
        if (len > 0)
        {
            t_vector* vec = vecnew(1, sizeof(uint8_t), 1);
            for (int i = 0; i < len; i++)
            {
                vecpush(vec, (const char*)&uart->data[i]);
            }
            /* rs_send(uart->port_num, (char*)vec->data, vec->length); */
            rs_rpc_parse_string(uart, vec);
            vecfree(vec);
        }
        else
        {
            ESP_ERROR_CHECK(uart_wait_tx_done(uart->port_num, 2));
        }
    }
    vTaskDelete(NULL);
}

esp_err_t first_init(app_t* app)
{
    esp_err_t err;

    if ((err = uart_init(&app->uart)))
        return (err);
    xTaskCreate(uart_task, "uart_task", 2048, &app->uart, 10, NULL);

    return (ESP_OK);
}
