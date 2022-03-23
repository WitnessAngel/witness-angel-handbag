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

#define TAG "first_init"

/* CONFIG_ECHO_UART_PORT_NUM = 2
   CONFIG_ECHO_UART_BAUD_RATE = 115200
   CONFIG_ECHO_UART_RXD = 3
   CONFIG_ECHO_UART_TXD = 1
   CONFIG_ECHO_UART_RTS = 22 */

static void rs_send(const int port, const char* str, size_t size)
{
    if (uart_write_bytes(port, str, size) != size)
    {
        ESP_LOGE(TAG, "Send data critical failure.");
        // add your code to handle sending failure here
        abort();
    }
}

/* static void rs_rpc_test(void) */
/* { */

/*     RPCJson_t request; */
/*     RPCJson_t testing; */

/*     rpc_json_create_request(&testing, "ping", 1); */
/*     char* jsontesting_string = cJSON_Print(testing.rpc_json_obj); */
/*     puts(jsontesting_string); */
/*     char test[] = "{\"jsonrpc\": \"2.0\", \"method\": \"ping\", \"id\": 40}";
 */
/*     puts(test); */
/*     char test_2[] = "{\n\ */
/* 	\"json_rpc\":	\"2.0\",\n\ */
/* 	\"method\":	\"ping\",\n\ */
/* 	\"id\":	1\n\ */
/* }"; */
/*     puts(test_2); */
/*     if (!rpc_json_parse(&request, (char*)test)) */
/*     { */
/*         ESP_LOGE(TAG, "rpc json parse fail"); */
/*         return; */
/*     } */
/* } */

static void rs_rpc_response(t_vector* input)
{
    RPCJson_t request;
    RPCJson_t response;
    int id = 0;

    /* char test[] = "{\"jsonrpc\": \"2.0\", \"method\": \"ping\", \"id\": 40}";
     */
    /* cJSON* testing = cJSON_Parse(input->data); */
    /* if (testing == NULL) */
    /* { */
    /*     rs_send(ECHO_UART_PORT, "\nError: 'parse json failed'", */
    /*             strlen("\nError: 'parse json failed'")); */
    /* } */

    /* rpc_json_create_request(&testing, "ping", 1); */
    /* char* jsontesting_string = cJSON_Print(testing.rpc_json_obj); */
    /* rs_send(ECHO_UART_PORT, jsontesting_string,
     * strlen(jsontesting_string));
     */
    /* if (!cJSON_Parse(test)) */
    /*     rs_send(ECHO_UART_PORT, "\nError: Test cJSON\n", */
    /*             strlen("\nError: Test cJSON\n")); */
    /* else */
    /*     rs_send(ECHO_UART_PORT, "\nOk: Test cJSON\n", */
    /*             strlen("\nOk: Test cJSON\n")); */
    /* if (!rpc_json_parse(&request, jsontesting_string)) */
    /*     rs_send(ECHO_UART_PORT, "\nError: Test\n", strlen("\nError:
     * Test\n")); */

    /* if (!rpc_json_parse(&request, (char*)input->data)) */
    /* { */
    /*     rs_send(ECHO_UART_PORT, "\nError: 'rpc json parse fail'", */
    /*             strlen("\nError: 'rpc json parse fail'")); */
    /*     return; */
    /* } */
    /* if (!rpc_json_get_id(&request, &id)) */
    /*     rs_send(ECHO_UART_PORT, "\nError:\nrpc json get fail", */
    /*             strlen("\nError:\nrpc json get fail")); */
    /* if (!rpc_json_create_response(&response, id, 42)) */
    /* { */
    /*     /\* rpc_json_create_error(&response, 0, 1, "rpc_json_parse error");
     */
    /*      *\/ */
    /*     ESP_LOGE(TAG, "rpc json create response fail"); */
    /*     rs_send(ECHO_UART_PORT, "\nError:\nrpc json create response fail", */
    /*             strlen("\nError:\nrpc json create response fail")); */
    /*     return; */
    /* } */
    /* char* json_string = cJSON_PrintUnformatted(response.rpc_json_obj); */
    /* rs_send(ECHO_UART_PORT, json_string, strlen(json_string)); */
}

// uart_task with hardware flow control on UART
static void uart_task(void* arg)
{
    uart_t* uart = (uart_t*)arg;

    ESP_LOGI(TAG, "UART start receive loop.\r\n");
    /* rs_send(uart_num, "Start RS485 UART test.\r\n", 24); */

    while (1)
    {
        // Read data from UART
        int len =
            uart_read_bytes(uart->port_num, uart->data, uart->buf_size,
                            1000 / portTICK_PERIOD_MS); // PACKET_READ_TICS

        // Write data back to UART
        if (len > 0)
        {
            /* rs_send(uart->port_num, "\r\n", 2); */
            /* char prefix[] = "RS485 Received: ["; */
            /* rs_send(uart_num, prefix, (sizeof(prefix) - 1)); */
            /* ESP_LOGI(TAG, "Received %u bytes:", len); */
            /* printf("[ "); */
            t_vector* vec = vecnew(1, sizeof(uint8_t), 1);
            /* char c[5] = {0}; */
            for (int i = 0; i < len; i++)
            {
                /* sprintf(c, "0x%.2X", (uint8_t)data[i]); */
                vecpush(vec, (const char*)&uart->data[i]);
                /* rs_send(uart_num, (const char*)&data[i], 1); */
                // Add a Newline character if you get a return charater from
                // paste (Paste tests multibyte receipt/buffer)
                /* if (data[i] == '\r') */
                /* { */
                /*     rs_send(uart_num, "\n", 1); */
                /* } */
            }
            /* rs_send(uart_num, (char*)data, len); */
            /* ESP_LOGI(TAG, "%s", (char*)vec->data); */
            /* char test[5] = {0}; */
            /* sprintf(test, "%d", vec->length); */
            /* rs_send(uart_num, test, strlen(test)); */
            rs_send(uart->port_num, (char*)vec->data, vec->length);
            rs_rpc_response(vec);
            vecfree(vec);
            /* printf("] \n"); */
            /* rs_send(uart_num, "]\r\n", 3); */
        }
        else
        {
            /* Echo a "." to show we are alive while we wait for input */
            /* rs_send(uart_num, ".", 1); */
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
