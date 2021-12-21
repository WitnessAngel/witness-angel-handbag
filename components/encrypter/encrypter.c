#include <esp_log.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

#include "encrypter.h"
#include "pkcs7_padding.h"
#include "utils.h"

static const char* TAG = "encrypter";

void encrypter_init(encrypter_t* self)
{
    bzero(self->memdesc, sizeof(struct memdesc) * 2);
    self->memdesc[0].ptr = (uint8_t*)malloc(sizeof(uint8_t) * 16);
    esp_aes_init(&self->ctx);

    strcpy((char*)self->key, "(H+MbQeThWmZq4t7w!z%C*F-J@NcRfUj");
    memset(self->iv, 0, sizeof(self->iv));

    esp_aes_setkey(&self->ctx, self->key, 256);
}

uint8_t* encrypter_crypt(encrypter_t* self, const uint8_t* data,
                         size_t* data_length)
{
    self->memdesc[1].length = *data_length;
    self->memdesc[1].ptr = (uint8_t*)data;
    *data_length += self->memdesc[0].length;
    uint8_t* concatenated_input =
        (uint8_t*)malloc(sizeof(uint8_t) * *data_length);
    if (concatenated_input == NULL)
    {
        *data_length = 0;
        ESP_LOGE(TAG, "Error: malloc failed");
        return (NULL);
    }
    memcat(concatenated_input, self->memdesc, 2, *data_length);
    size_t crypted_length = find_prev_multiple_of(16, *data_length);
    uint8_t* crypted_data = (uint8_t*)malloc(sizeof(uint8_t) * crypted_length);
    if (crypted_data == NULL)
    {
        *data_length = 0;
        free(concatenated_input);
        ESP_LOGE(TAG, "Error: malloc failed");
        return (NULL);
    }

    esp_aes_crypt_cbc(&self->ctx, ESP_AES_ENCRYPT, crypted_length, self->iv,
                      (const uint8_t*)concatenated_input,
                      (uint8_t*)crypted_data);
    if (crypted_length != *data_length)
    {
        self->memdesc[0].length = *data_length - crypted_length;
        uint8_ncpy(self->memdesc[0].ptr, &concatenated_input[crypted_length],
                   self->memdesc[0].length, 16);
        *data_length = crypted_length;
    }
    else
        self->memdesc[0].length = 0;

    free(concatenated_input);
    return (crypted_data);
}

uint8_t* encrypter_finish(encrypter_t* self, size_t* data_length)
{
    int pad = pkcs7_padding_pad_buffer(self->memdesc[0].ptr,
                                       self->memdesc[0].length, 16, 16);
    *data_length = self->memdesc[0].length + pad;

    uint8_t* crypted_data = (uint8_t*)malloc(sizeof(uint8_t) * *data_length);
    if (crypted_data == NULL)
    {
        *data_length = 0;
        free(self->memdesc[0].ptr);
        esp_aes_free(&self->ctx);
        ESP_LOGE(TAG, "Error: malloc failed");
        return (NULL);
    }

    esp_aes_crypt_cbc(&self->ctx, ESP_AES_ENCRYPT, *data_length, self->iv,
                      (const uint8_t*)self->memdesc[0].ptr,
                      (uint8_t*)crypted_data);
    free(self->memdesc[0].ptr);
    esp_aes_free(&self->ctx);
    return (crypted_data);
}
