#include "vector.h"

void vecfree(t_vector* vec)
{
    if (vec)
    {
        free(vec->data);
        vec->data = 0;
        free(vec);
    }
}
