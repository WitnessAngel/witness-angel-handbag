#pragma once

#include <aes/esp_aes.h>

typedef struct encrypter_s encrypter_t;
struct encrypter_s
{
    uint8_t key[32];
    uint8_t iv[16];
    size_t buffer_length;
    size_t buffer_size;
    uint8_t* buffer;
    esp_aes_context ctx;
};

void encrypter_init(encrypter_t* self, size_t buffer_size,
                    size_t buffer_length);

void encrypter_crypt(encrypter_t* self, void** data);
void* encrypter_crypt_specific(encrypter_t* self, const void* data,
                               size_t* data_size, size_t* data_length);

void encrypter_finish(encrypter_t* self);
