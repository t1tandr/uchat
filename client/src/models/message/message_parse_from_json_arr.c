#include "uchat.h"

t_list* message_parse_from_json_arr(cJSON* arr) {
    if (arr == NULL || !cJSON_IsArray(arr)) {
        return NULL;
    }

    t_list* list = NULL;

    for(int i = 0 ; i < cJSON_GetArraySize(arr); i++) {
        t_message* message = message_parse_from_json(cJSON_GetArrayItem(arr, i));
        
        if (message != NULL) {
            mx_push_back(&list, message);
        } 
    }

    return list;
}

