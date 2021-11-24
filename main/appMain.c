#include <esp_err.h>
#include <esp_log.h>

#include "app.h"
#include "camera.h"
#include "esp_camera.h"
#include "i2sMicro.h"
#include "noVolatileStorage.h"
#include "sdCard.h"

static const char *TAG = "app_main";

static esp_err_t app_init(app_t *self)
{
  esp_err_t err;

  if ((err = nvs_init())
	  || (err = sdCard_init())
	  || (err = i2sMicro_init(&self->i2sMicro))
	  || (err = camera_init()))
  {
    ESP_LOGE(TAG, "Error during app init : (%s) !\n", esp_err_to_name(err));
    return (err);
  }

  ESP_LOGI(TAG, "App init completed with success !\n");
  return (ESP_OK);
}

void app_main()
{
	static app_t app = {0};
	/* esp_err_t err; */

	ESP_ERROR_CHECK(app_init(&app));

	ESP_LOGI(TAG, "App start !");

	micro_record(&app, "/sdcard/test.wav");

	ESP_LOGI(TAG, "App end !");
}
