#pragma once

#include <aes/esp_aes.h>

#include "encrypter.h"

#pragma pack(push, 1)
typedef struct _encrypted_wav_header
{
    // RIFF Header
    char riff_header[4]; // Contains "RIFF"
    int wav_size; // Size of the wav portion of the file, which follows the
                  // first 8 bytes. File size - 8
    char wave_header[4]; // Contains "WAVE"

    // Format Header
    char fmt_header[4]; // Contains "fmt " (includes trailing space)
    int fmt_chunk_size; // Should be 16 for PCM
    short audio_format; // Should be 1 for PCM. 3 for IEEE Float
    short num_channels;
    int sample_rate;
    int byte_rate; // Number of bytes per second. sample_rate * num_channels *
                   // Bytes Per Sample
    short sample_alignment; // num_channels * Bytes Per Sample
    short bit_depth;        // Number of bits per sample

    // Data
    char data_header[4]; // Contains "data"
    int data_bytes; // Number of bytes in data. Number of samples * num_channels
                    // * sample byte size
                    // uint8_t bytes[]; // Remainder of wave file is bytes
} encrypted_wav_header_t;
#pragma pack(pop)

typedef struct encryptedWavFile_s encryptedWavFile_t;
struct encryptedWavFile_s
{
    size_t file_size;
    FILE* fd;
    encrypted_wav_header_t header;
    size_t buffer_length;
    encrypter_t encrypter;
};

void encryptedWavFile_init(encryptedWavFile_t* self, FILE* fd, int sample_rate,
                           size_t buffer_length);
void encryptedWavFile_write(encryptedWavFile_t* self, int16_t** samples);
void encryptedWavFile_finish(encryptedWavFile_t* self);
