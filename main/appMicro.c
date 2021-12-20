#include <esp_log.h>
#include <esp_timer.h>

#include "app.h"
#include "encryptedWavFile.h"
#include "i2sMicro.h"
#include "wavFile.h"

/* void record(I2SSampler *input, const char *fname) { */

#define BUFFER_LENGTH 1024

static const char* TAG = "appMicro";

void record(app_t* app, const char* fname)
{
    wavFile_t wavFile;
    encryptedWavFile_t encryptedWavFile;

    int16_t* samples = (int16_t*)malloc(sizeof(int16_t) * BUFFER_LENGTH);
    ESP_LOGI(TAG, "Start recording");
    i2sMicro_start(&app->i2sMicro);
    // open the file on the sdcard
    FILE* fd = fopen(fname, "wb");
    FILE* fd_encrypt = fopen("/sdcard/encrypted.wav", "wb");
    if (fd == NULL || fd_encrypt == NULL)
        ESP_LOGE(TAG, "Could not open file");
    // create a new wave file writer
    wavFile_init(&wavFile, fd, app->i2sMicro.sample_rate);
    encryptedWavFile_init(&encryptedWavFile, fd_encrypt,
                          app->i2sMicro.sample_rate, BUFFER_LENGTH);
    size_t i = 0;
    while (i < 100)
    {
        int samples_read =
            i2sMicro_read(&app->i2sMicro, samples, BUFFER_LENGTH);
        int64_t start = esp_timer_get_time();
        wavFile_write(&wavFile, samples, samples_read);
        encryptedWavFile_write(&encryptedWavFile, &samples);
        int64_t end = esp_timer_get_time();
        ESP_LOGI(TAG, "Wrote %d samples in %lld microseconds", samples_read,
                 end - start);
        i++;
    }
    // stop the input
    i2sMicro_stop(&app->i2sMicro);
    // and finish the writing
    wavFile_finish(&wavFile);
    encryptedWavFile_finish(&encryptedWavFile);
    fclose(fd);
    fclose(fd_encrypt);
    free(samples);
    ESP_LOGI(TAG, "Finished recording");
}
