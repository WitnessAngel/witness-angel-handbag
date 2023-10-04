#include <esp_log.h>
#include <stdlib.h>
#include <string.h>

#include "padding.h"
#include "pkcs7_padding.c"

/* static const char* TAG = "padding"; */

size_t padding_pad_buffer(const uint8_t* input_buffer, size_t buffer_length,
                          uint8_t** output_buffer)
{
    size_t result_length = buffer_length;

    if (result_length % 16)
    {
        result_length += 16 - (buffer_length % 16);
        *output_buffer = (uint8_t*)malloc(sizeof(uint8_t) * result_length);
        if (*output_buffer == NULL)
            return 0;
        memcpy((void*)*output_buffer, (void*)input_buffer, buffer_length);
        int pad = pkcs7_padding_pad_buffer(*output_buffer, buffer_length,
                                           result_length, 16);
        /* ESP_LOGI(TAG, "pad = %d", pad); */
        return (result_length);
    }
    else
        return (result_length);
}

size_t padding_pad_file(uint8_t** output, size_t file_size)
{
    size_t padding_length = 0;

    if (file_size % 16)
    {
        padding_length = 16 - (file_size % 16);
        uint8_t* padding = (uint8_t*)malloc(sizeof(uint8_t) * padding_length);
        if (padding == NULL)
            return 0;
        int pad = pkcs7_padding_pad_buffer(*output, 0, padding_length, 16);
        /* ESP_LOGI(TAG, "pad = %d", pad); */
        return (padding_length);
    }
    else
        return (padding_length);
}
