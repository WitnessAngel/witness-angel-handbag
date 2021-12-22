#include <esp_log.h>
#include <stdio.h>

#include <bootloader_random.h>
#include <esp_random.h>

#include <mbedtls/pk.h>
#include <mbedtls/rsa.h>

#include "encrypter.h"

static const char* TAG = "encrypter_generation";

void key_generation(encrypter_t* self)
{
    /* bootloader_random_enable(); */
    esp_fill_random(self->key, 32);
    /* for (int i = 0; i < 32; i++) */
    /* { */
    /*     ESP_LOGI(TAG, "KEY: %x", self->key[i]); */
    /* } */
    esp_fill_random(self->iv, 16);
    /* for (int i = 0; i < 16; i++) */
    /* { */
    /*     ESP_LOGI(TAG, "IV: %x", self->iv[i]); */
    /* } */

    int ret = 0;
    mbedtls_pk_context pk;
    mbedtls_pk_init(&pk);

    /*
     * Read the RSA public key
     */

    if ((ret = mbedtls_pk_parse_public_keyfile(&pk, "/sdcard/publicKey.pem")) !=
        0)
        ESP_LOGE(TAG, "mbedtls_pk_parse_public_keyfile returned -0x%04x\n",
                 -ret);
    /* bootloader_random_disable(); */
}

/* mbedtls_rsa_context ctx; */
/* mbedtls_rsa_init(&ctx, MBEDTLS_RSA_PKCS_V21, 10); */
/* mbedtls_rsa_free(&ctx); */
/* mbedtls_rsa_rsaes_oaep_encrypt */
