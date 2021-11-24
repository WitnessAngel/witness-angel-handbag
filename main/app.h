#pragma once

#include <esp_err.h>

#include "aviFile.h"
#include "camera.h"
#include "esp_camera.h"
#include "sdCard.h"
#include "wavFile.h"
#include "i2sMicro.h"

typedef struct app_s app_t;
struct app_s {
	wavFile_t wav;
	aviFile_t avi;
	i2sMicro_t i2sMicro;
};

void micro_record(app_t *app, const char *fname);
