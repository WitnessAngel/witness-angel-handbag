#include <aes/esp_aes.h>
#include <esp_log.h>
#include <stdio.h>
#include <string.h>

#include "encryptedWavFile.h"

static const char* TAG = "encryptedWavFile";

void encryptedWavFile_init(encryptedWavFile_t* self, FILE* fd, int sample_rate)
{
    strcpy((char*)self->key, "(H+MbQeThWmZq4t7w!z%C*F-J@NcRfUj");
    memset(self->iv, 0, sizeof(self->iv));
    esp_aes_init(&self->ctx);
    esp_aes_setkey(&self->ctx, self->key, 256);

    strcpy(self->header.riff_header, "RIFF");
    self->header.wav_size = 0;
    strcpy(self->header.wave_header, "WAVE");
    strcpy(self->header.fmt_header, "fmt ");
    self->header.fmt_chunk_size = 16;
    self->header.audio_format = 1;
    self->header.num_channels = 1;
    self->header.sample_rate = sample_rate;
    self->header.byte_rate = 32000;
    self->header.sample_alignment = 2;
    self->header.bit_depth = 16;
    strcpy(self->header.data_header, "data");
    self->header.data_bytes = 0;
    self->fd = fd;
    // write out the header - we'll fill in some of the blanks later
    fwrite(&self->header, sizeof(encrypted_wav_header_t), 1, self->fd);
    self->file_size = sizeof(encrypted_wav_header_t);
}

void encryptedWavFile_write(encryptedWavFile_t *self, int16_t *samples,
                            int16_t *encrypted_samples, int count) {
  esp_aes_crypt_cbc(&self->ctx, ESP_AES_ENCRYPT, count, self->iv,
                    (uint8_t *)samples, (uint8_t *)encrypted_samples);
  // write the samples and keep track of the file size so far
  fwrite(encrypted_samples, sizeof(int16_t), count, self->fd);
  self->file_size += sizeof(int16_t) * count;
}

void encryptedWavFile_finish(encryptedWavFile_t* self)
{
    ESP_LOGI(TAG, "WAV file size: %d", self->file_size);
    // now fill in the header with the correct information and write it again
    self->header.data_bytes = self->file_size - sizeof(encrypted_wav_header_t);
    self->header.wav_size = self->file_size - 8;
    fseek(self->fd, 0, SEEK_SET);
    fwrite(&self->header, sizeof(encrypted_wav_header_t), 1, self->fd);

    esp_aes_free(&self->ctx);
}
