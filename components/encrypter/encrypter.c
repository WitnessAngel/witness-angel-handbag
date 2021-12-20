#include <esp_log.h>
#include <stdio.h>
#include <string.h>

#include "encrypter.h"

static const char* TAG = "encrypter";

void encrypter_init(encrypter_t* self, size_t buffer_size, size_t buffer_length)
{
    self->buffer_size = buffer_size;
    self->buffer_length = buffer_length;
    esp_aes_init(&self->ctx);

    strcpy((char*)self->key, "(H+MbQeThWmZq4t7w!z%C*F-J@NcRfUj");
    memset(self->iv, 0, sizeof(self->iv));

    esp_aes_setkey(&self->ctx, self->key, 256);
    self->buffer = (uint8_t*)malloc(buffer_size * buffer_length);
}

void* encrypter_crypt_specific(encrypter_t* self, const void* data,
                               size_t data_size, size_t data_length)
{
    uint8_t* crypted_data = (uint8_t*)malloc(data_size * data_length);
    esp_aes_crypt_cbc(&self->ctx, ESP_AES_ENCRYPT, data_size * data_length,
                      self->iv, (const uint8_t*)data, (uint8_t*)crypted_data);
    return ((void*)crypted_data);
}

void encrypter_crypt(encrypter_t* self, void** data)
{
    uint8_t* tmp = (uint8_t*)*data;
    esp_aes_crypt_cbc(&self->ctx, ESP_AES_ENCRYPT,
                      self->buffer_size * self->buffer_length, self->iv,
                      (const uint8_t*)*data, (uint8_t*)self->buffer);
    *data = (void*)self->buffer;
    self->buffer = (uint8_t*)tmp;
}

void encrypter_finish(encrypter_t* self)
{
    free(self->buffer);
    esp_aes_free(&self->ctx);
}
