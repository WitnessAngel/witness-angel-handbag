/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>

#include "CApi.h"

bool validate_int32_param_in_c(int param, int value)
{
    printf("C validated param #%d = %d\n", param, value);
    return true;
}

bool validate_str_param_in_c(int param, char* value)
{
    printf("C validated param #%d = %s\n", param, value);
    return true;
}
