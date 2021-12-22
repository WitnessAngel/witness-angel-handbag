#pragma once

#include <aes/esp_aes.h>

#include "utils.h"

/**
 * @brief Encrypter context structure
 *
 */
typedef struct encrypter_s
{
    uint8_t key[32];
    uint8_t iv[16];
    struct memdesc memdesc[2];
    uint8_t buffer[16];
    esp_aes_context ctx;
} encrypter_t;

/**
 * @brief This function initializes an encrypter context.
 *
 * @param self			The encrypter context to initializes
 *
 */
void encrypter_init(encrypter_t* self);

/**
 * @brief Crypt the first 16n array of data with aes_cbc.
 *
 * @param self				The encrypter context
 *
 * @param data				The data to crypt
 *
 * @param data_length		The length of the data input/output.
 *
 * @return
 *     - MALLOCED_CRYPTED_DATA              Success
 *	   - NULL Failure
 */
uint8_t* encrypter_crypt(encrypter_t* self, const uint8_t* data,
                         size_t* data_length);

/**
 * @brief This function free the components of an encrypter context. And crypt
 *the last data buffer with the padding.
 *
 * @param self			The encrypter context to free
 *
 * @param data_length	The length of the output crypted data.
 *
 * @return
 *     - MALLOCED_CRYPTED_DATA              Success
 *	   - NULL Failure
 */
uint8_t* encrypter_finish(encrypter_t* self, size_t* data_length);

void key_generation(encrypter_t* self);
