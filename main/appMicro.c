#include <esp_log.h>
#include <esp_timer.h>

#include "app.h"
#include "i2sMicro.h"
#include "wavFile.h"

/* void record(I2SSampler *input, const char *fname) { */

static const char *TAG = "appMicro";

void record(app_t *app, const char *fname)
{
	wavFile_t wavFile;
	wavFile_t wavFile_encrypt;

    int16_t *samples = (int16_t *)malloc(sizeof(int16_t) * 1024);
    int16_t *encrypted = (int16_t *)malloc(sizeof(int16_t) * 1024);
    ESP_LOGI(TAG, "Start recording");
    i2sMicro_start(&app->i2sMicro);
    // open the file on the sdcard
    FILE *fd = fopen(fname, "wb");
	FILE *fd_encrypt = fopen("/sdcard/encrypted.wav", "wb");
	if (fd == NULL || fd_encrypt == NULL)
		ESP_LOGE(TAG, "Could not open file");
    // create a new wave file writer
    wavFile_init(&wavFile, fd, app->i2sMicro.sample_rate);
    wavFile_init(&wavFile_encrypt, fd_encrypt, app->i2sMicro.sample_rate);
    // keep writing until the user releases the button
    size_t i = 0;
    while (i < 200) {
		int samples_read = i2sMicro_read(&app->i2sMicro, samples, 1024);
		/* encrypt(samples, encrypted, sizeof(int16_t) * samples_read); */
		encodetest(samples, encrypted, samples_read * sizeof(int16_t));
		int64_t start = esp_timer_get_time();
		wavFile_write(&wavFile, samples, samples_read);
		wavFile_write(&wavFile_encrypt, encrypted, samples_read);
		int64_t end = esp_timer_get_time();
		ESP_LOGI(TAG, "Wrote %d samples in %lld microseconds", samples_read, end - start);
		i++;
    }
    // stop the input
    i2sMicro_stop(&app->i2sMicro);
    // and finish the writing
    wavFile_finish(&wavFile);
    wavFile_finish(&wavFile_encrypt);
    fclose(fd);
    free(samples);
    ESP_LOGI(TAG, "Finished recording");
  }
