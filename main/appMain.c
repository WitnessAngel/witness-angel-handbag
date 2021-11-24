#include <esp_err.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "app.h"
#include "sdCard.h"
#include "noVolatileStorage.h"
#include "i2sMicro.h"

static const char *TAG = "appMain";


static esp_err_t app_init(app_t *app) {
  esp_err_t err;

  if ((err = nvs_init())
      /* || (err = appScreen_init(&app->screen)) */
      /* || (err = esp_netif_init()) */
      /* || (err = esp_event_loop_create_default())) */
      /* || (err = multicastDNS_init()) */
      /* || (err = appServer_init(app)) */
      || (err = sdCard_init())
	  || (err = i2sMicro_init(&app->i2sMicro)))
  /* || (err = wifi_init(&app->wifi)) */
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
	ESP_LOGI(TAG, "appMicro begin");
	record(&app, "/sdcard/manu.wav");
}
