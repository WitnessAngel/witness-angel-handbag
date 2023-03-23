#pragma once

#include <stdlib.h>
#include <string.h>

typedef struct s_vector
{
    void* data;
    size_t capacity;
    size_t length;
    size_t realloc_size;
    size_t data_size;
} t_vector;

t_vector* vecnew(size_t def_alloc, size_t data_size, size_t reall_size);
void vecpush(t_vector* vec, const void* data);
void vecfree(t_vector* vec);
void vecalloc(t_vector* vec, size_t alloc_size);
t_vector* veccpy(const t_vector* const vec);
