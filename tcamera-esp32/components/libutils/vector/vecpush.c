#include "vector.h"

void vecpush(t_vector* vec, const void* data)
{
    void* tmp;

    if (!vec)
        return;
    if (vec->capacity == vec->length)
    {
        tmp = vec->data;
        vec->data = calloc((vec->capacity + vec->realloc_size), vec->data_size);
        if (!vec->data)
        {
            vec->data = tmp;
            return;
        }
        memcpy(vec->data, tmp, vec->data_size * vec->capacity);
        vec->capacity += vec->realloc_size;
        free(tmp);
    }
    memcpy(vec->data + vec->length++ * vec->data_size, data, vec->data_size);
}
