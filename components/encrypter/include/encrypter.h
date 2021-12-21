#pragma once

#include <aes/esp_aes.h>

#include "utils.h"

typedef struct encrypter_s encrypter_t;
struct encrypter_s
{
    uint8_t key[32];
    uint8_t iv[16];
    struct memdesc memdesc[2];
    uint8_t buffer[16];
    esp_aes_context ctx;
};

void encrypter_init(encrypter_t* self);

/**
 * @brief Crypt the first 16n array of data with aes_cbc.
 *
 * @param self         Encrypter instance
 *
 * @param data      Data to crypt
 *
 * @param data_length      Length of the data input/output.
 *
 * @return
 *     - MALLOCED_CRYPTED_DATA              Success
 *	   - NULL Failure
 */
uint8_t* encrypter_crypt(encrypter_t* self, const uint8_t* data,
                         size_t* data_length);

uint8_t* encrypter_finish(encrypter_t* self, size_t* data_length);
