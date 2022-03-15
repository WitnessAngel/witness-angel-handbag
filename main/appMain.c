#include <esp_err.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "app.h"
#include "i2sMicro.h"
#include "noVolatileStorage.h"
#include "sdCard.h"

static const char* TAG = "appMain";

static esp_err_t app_init(app_t* app)
{
    esp_err_t err;

    if ((err = nvs_init()))
    /* || (err = sdCard_init()) */
    /* || (err = i2sMicro_init(&app->i2sMicro))) */
    {
        ESP_LOGE(TAG, "Error during app init : (%s) !\n", esp_err_to_name(err));
        return (err);
    }

    ESP_LOGI(TAG, "App init completed with success !\n");
    return (ESP_OK);
}

/* #include <esp_heap_trace.h> // debug */
/* #define NUM_RECORDS 1000 */
/* static heap_trace_record_t */
/*     trace_record[NUM_RECORDS]; // This buffer must be in internal RAM */

void app_main(void)
{
    static app_t app = {0};

    ESP_ERROR_CHECK(app_init(&app));
    /* TEST */
    first_init(&app);

    /* END TEST */
    /* ESP_ERROR_CHECK(heap_trace_init_standalone(trace_record, NUM_RECORDS));
     */

    ESP_LOGI(TAG, "appMicro begin");

    /* ESP_LOGI(TAG, "Free heap at startup : %u", */
    /*          heap_caps_get_free_size(MALLOC_CAP_8BIT)); */

    /* record(&app, "/sdcard/encrypted.wav"); */

    /* ESP_LOGI(TAG, "Free heap at end : %u", */
    /*          heap_caps_get_free_size(MALLOC_CAP_8BIT)); */
}
