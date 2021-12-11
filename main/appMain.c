#include <esp_err.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "app.h"
#include "i2sMicro.h"
#include "noVolatileStorage.h"
#include "sdCard.h"

#include "RustApi.h"

static const char* TAG = "appMain";

static esp_err_t app_init(app_t* app)
{
    esp_err_t err;

    if ((err = nvs_init()) || (err = sdCard_init()) ||
        (err = i2sMicro_init(&app->i2sMicro)))
    {
        ESP_LOGE(TAG, "Error during app init : (%s) !\n", esp_err_to_name(err));
        return (err);
    }

    ESP_LOGI(TAG, "App init completed with success !\n");
    return (ESP_OK);
}

void app_main(void)
{
    static app_t app = {0};

    ESP_ERROR_CHECK(app_init(&app));

    ESP_LOGI(TAG, "rust part begin");
    int rust_ret = create_file("/sdcard/rust.txt");
    ESP_LOGI(TAG, "create file return %d", rust_ret);
    ESP_LOGI(TAG, "rust part end");

	ESP_ERROR_CHECK(app_init(&app));
	ESP_LOGI(TAG, "appMicro begin");
	record(&app, "/sdcard/manu.wav");
}
