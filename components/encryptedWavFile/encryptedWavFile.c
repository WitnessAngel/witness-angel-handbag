#include <esp_log.h>
#include <stdio.h>
#include <string.h>

#include "encryptedWavFile.h"
#include "encrypter.h"

static const char* TAG = "encryptedWavFile";

void encryptedWavFile_init(encryptedWavFile_t* self, FILE* fd, int sample_rate,
                           size_t file_size)
{
    encrypter_init(&self->encrypter);

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
    self->header.data_bytes = file_size - sizeof(encrypted_wav_header_t);
    self->header.wav_size = file_size - 8;

    self->fd = fd;

    size_t header_size = sizeof(encrypted_wav_header_t);

    uint8_t* encrypted_header = encrypter_crypt(
        &self->encrypter, (uint8_t*)&self->header, &header_size);
    fwrite(encrypted_header, sizeof(uint8_t), header_size, self->fd);
    free(encrypted_header);

    self->file_size = header_size;
}

void encryptedWavFile_write(encryptedWavFile_t* self, const int16_t* samples,
                            size_t samples_length)
{
    samples_length *= sizeof(int16_t);
    uint8_t* encrypted_samples = encrypter_crypt(
        &self->encrypter, (const uint8_t*)samples, &samples_length);
    fwrite(encrypted_samples, sizeof(uint8_t), samples_length, self->fd);
    self->file_size += samples_length;
    free(encrypted_samples);
}

void encryptedWavFile_finish(encryptedWavFile_t* self)
{
    size_t samples_length = 0;
    uint8_t* remain_encrypted_samples =
        encrypter_finish(&self->encrypter, &samples_length);
    fwrite(remain_encrypted_samples, sizeof(uint8_t), samples_length, self->fd);
    self->file_size += samples_length;
    free(remain_encrypted_samples);

    ESP_LOGI(TAG, "WAV file size : %d", self->file_size);
}
