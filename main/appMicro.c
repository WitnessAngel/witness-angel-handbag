/* #include <esp_heap_trace.h> // debug */
#include <esp_log.h>
#include <esp_timer.h>
#include <math.h>

#include "app.h"
#include "encryptedWavFile.h"
#include "i2sMicro.h"

#define BUFFER_LENGTH 1024
#define DURATION_SEC 30
#define ITERATION roundf(15.625 * DURATION_SEC)

static const char* TAG = "appMicro";

void record(app_t* app, const char* fname)
{
    /* ESP_ERROR_CHECK(heap_trace_start(HEAP_TRACE_LEAKS)); */

    encryptedWavFile_t encryptedWavFile;

    int16_t* samples = (int16_t*)malloc(sizeof(int16_t) * BUFFER_LENGTH);
    ESP_LOGI(TAG, "Start recording");
    i2sMicro_start(&app->i2sMicro);
    // open the file on the sdcard
    FILE* fd = fopen(fname, "wb");
    if (fd == NULL)
        ESP_LOGE(TAG, "Could not open file");
    // create a new wave file writer
    encryptedWavFile_init(&encryptedWavFile, fd, app->i2sMicro.sample_rate,
                          (ITERATION * (BUFFER_LENGTH * 2)) + 44);
    size_t i = 0;
    while (i < ITERATION)
    {
        int samples_read =
            i2sMicro_read(&app->i2sMicro, samples, BUFFER_LENGTH);
        /* int64_t start = esp_timer_get_time(); */
        encryptedWavFile_write(&encryptedWavFile, samples, samples_read);
        /* int64_t end = esp_timer_get_time(); */
        /* ESP_LOGI(TAG, "Wrote %d samples in %lld microseconds", samples_read,
         */
        /*          end - start); */
        i++;
    }
    // stop the input
    i2sMicro_stop(&app->i2sMicro);
    // and finish the writing
    encryptedWavFile_finish(&encryptedWavFile);
    fclose(fd);
    free(samples);
    /* ESP_ERROR_CHECK(heap_trace_stop()); */
    /* heap_trace_dump(); */
    ESP_LOGI(TAG, "Finished recording");
}
