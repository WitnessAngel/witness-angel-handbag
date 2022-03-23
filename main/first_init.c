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

void encrypted_uart_init(encrypted_uart_t* encrypted_uart,
                         const unsigned char* key, const size_t keylen)
{
    int ret = 0;
    mbedtls_pk_init(&encrypted_uart->pk);

    /*
     * Read the RSA public key
     */
    if ((ret = mbedtls_pk_parse_public_key(&encrypted_uart->pk, key, keylen)) !=
        0)
    {
        ESP_LOGE(
            TAG,
            " failed\n  ! mbedtls_pk_parse_public_keyfile returned -0x%04x\n",
            -ret);
    }

    mbedtls_entropy_init(&encrypted_uart->entropy);
    mbedtls_ctr_drbg_init(&encrypted_uart->drbg);

    int pub_ret;
    const char* pers = "encrypted_uart_communication";

    /* Seed the PRNG using the entropy pool, and throw in our secret key as an
     * additional source of randomness. */

    pub_ret = mbedtls_ctr_drbg_seed(&encrypted_uart->drbg, mbedtls_entropy_func,
                                    &encrypted_uart->entropy,
                                    (const unsigned char*)pers, strlen(pers));

    if (pub_ret != 0)
    {
        ESP_LOGE(TAG,
                 "Mauvaise configuration de la fonction mbedtls_ctr_drbg_seed");
    }

    mbedtls_ctr_drbg_random(&encrypted_uart->drbg, encrypted_uart->buf_drbg,
                            256);
}

void encrypt(encrypted_uart_t* encrypted_uart, const unsigned char* input,
             unsigned char* output, size_t input_length, size_t output_length)
{
    int ret;

    if ((ret = mbedtls_pk_encrypt(&encrypted_uart->pk, input, input_length,
                                  output, &output_length, sizeof(output),
                                  mbedtls_ctr_drbg_random,
                                  &encrypted_uart->drbg)) != 0)
    {
        ESP_LOGE(TAG, " failed\n  ! mbedtls_pk_encrypt returned -0x%04x\n",
                 -ret);
    }
}

void decrypt(encrypted_uart_t* encrypted_uart, const unsigned char* input,
             unsigned char* output, size_t input_length, size_t output_length)
{
    int ret;

    if ((ret = mbedtls_pk_decrypt(&encrypted_uart->pk, input, input_length,
                                  output, &output_length, sizeof(output),
                                  mbedtls_ctr_drbg_random,
                                  &encrypted_uart->drbg)) != 0)
    {
        ESP_LOGE(TAG, " failed\n  ! mbedtls_pk_decrypt returned -0x%04x\n",
                 -ret);
    }
}

static void rs_send(const int port, const char* str, size_t size)
{
    if (uart_write_bytes(port, str, size) != size)
    {
        ESP_LOGE(TAG, "Send data critical failure.");
        // add your code to handle sending failure here
        abort();
    }
}

static void rs_rpc_response(uart_t* uart, t_vector* input)
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

    if (!rpc_json_parse(&request, (char*)input->data))
    {
        rs_send(uart->port_num, "\nError: 'rpc json parse fail'",
                strlen("\nError: 'rpc json parse fail'"));
        return;
    }
    if (!rpc_json_get_id(&request, &id))
        rs_send(uart->port_num, "\nError:\nrpc json get fail",
                strlen("\nError:\nrpc json get fail"));
    if (!rpc_json_create_response(&response, id, 42))
    {
        /* rpc_json_create_error(&response, 0, 1, "rpc_json_parse error");
         */
        ESP_LOGE(TAG, "rpc json create response fail");
        rs_send(uart->port_num, "\nError:\nrpc json create response fail",
                strlen("\nError:\nrpc json create response fail"));
        return;
    }
    char* json_string = cJSON_PrintUnformatted(response.rpc_json_obj);
    rs_send(uart->port_num, json_string, strlen(json_string));
}

// uart_task with hardware flow control on UART
static void uart_task(void* arg)
{
    uart_t* uart = (uart_t*)arg;
    encrypted_uart_t receive;
    encrypted_uart_t send;

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
            rs_rpc_response(uart, vec);
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
