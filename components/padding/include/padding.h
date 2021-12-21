#ifndef PADDING_H
#define PADDING_H

#include <stdio.h>

size_t padding_pad_buffer(const uint8_t* input_buffer, size_t buffer_length,
                          uint8_t** output_buffer);

size_t padding_pad_file(uint8_t** output, size_t file_size);

#endif /* PADDING_H */
