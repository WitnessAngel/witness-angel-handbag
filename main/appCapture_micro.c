#include <stdio.h>
#include <esp_log.h>

#include "app.h"
#include "wavFile.h"
#include "i2sMicro.h"

static const char *TAG = "appCapture_micro";

void micro_record(app_t *app, const char *fname)
{
	wavFile_t wavFile;

	int16_t *samples = (int16_t *)malloc(sizeof(int16_t) * 1024);
	if (samples == NULL)
		return ;
	ESP_LOGI(TAG, "Start recording");
	i2sMicro_start(&app->i2sMicro);
	// open the file on the sdcard
	FILE *fd = fopen(fname, "wb");
	if (fd == NULL)
          ESP_LOGE(TAG, "Could not open file");
	// create a new wave file writer
	wavFile_init(&wavFile, fd, app->i2sMicro.sample_rate);
	// keep writing until the user releases the button
	size_t i = 0;
	while (i < 500) {
		int samples_read = i2sMicro_read(&app->i2sMicro, samples, 1024);
		int64_t start = esp_timer_get_time();
		wavFile_write(&wavFile, samples, samples_read);
		int64_t end = esp_timer_get_time();
		ESP_LOGI(TAG, "Wrote %d samples in %lld microseconds", samples_read, end - start);
		i++;
	}
	// stop the input
	i2sMicro_stop(&app->i2sMicro);
	// and finish the writing
	wavFile_finish(&wavFile);
	fclose(fd);
	free(samples);
	ESP_LOGI(TAG, "Finished recording");
}
