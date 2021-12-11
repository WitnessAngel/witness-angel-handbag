/*
 * C functions exposed to Rust.
 */

#include <stdbool.h>

extern bool validate_int32_param_in_c(int param, int value);
extern bool validate_str_param_in_c(int param, char* value);
