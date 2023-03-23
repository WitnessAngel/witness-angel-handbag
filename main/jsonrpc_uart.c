#include "app.h"

#include "cjsonRpc.h"
#include "sdCard.h"
#include <cJSON.h>

#include <string.h>

// has to be in the same order as the enum declaration
const char* method_table[] = {"ping", "status", "add_aes_cbc_symkey"};

jsonrpc_method_t get_method_from(const char* method)
{
    for (size_t i = 0; i < sizeof(method_table); i++)
    {
        if (!strcmp(method, method_table[i]))
            return (i);
    }
    return (0);
}

RPCJson_t rs_rpc_parse_ping(const int id)
{
    RPCJson_t response;
    cJSON* result = cJSON_CreateObject();

    cJSON* default_str = cJSON_CreateString("pong");
    cJSON_AddItemToObject(result, "default", default_str);

    rpc_json_create_response_from_json_object(&response, id, result);

    return (response);
}

RPCJson_t rs_rpc_parse_status(const int id)
{
    RPCJson_t response;
    cJSON* result = cJSON_CreateObject();

    cJSON* available_symkey_count =
        cJSON_CreateNumber(get_count_of_files_in("/sdcard"));
    cJSON_AddItemToObject(result, "available_symkey_count",
                          available_symkey_count);
    cJSON* cryptainer_data_count =
        cJSON_CreateNumber(get_count_of_files_in("/sdcard"));
    cJSON_AddItemToObject(result, "cryptainer_data_count",
                          cryptainer_data_count);
    cJSON* disk_free_space_bytes =
        cJSON_CreateNumber(sdCard_get_free_spaces_bytes("/sdcard"));
    cJSON_AddItemToObject(result, "disk_free_space_bytes",
                          disk_free_space_bytes);
    cJSON* disk_total_space_bytes =
        cJSON_CreateNumber(sdCard_get_total_spaces_bytes("/sdcard"));
    cJSON_AddItemToObject(result, "disk_total_space_bytes",
                          disk_total_space_bytes);

    rpc_json_create_response_from_json_object(&response, id, result);

    return (response);
}

RPCJson_t rs_rpc_parse_add_aes_cbc_symkey(const int id, RPCJson_t* request)
{
    RPCJson_t response;
    rpc_json_create_response(&response, id, 42);

    return (response);
}
