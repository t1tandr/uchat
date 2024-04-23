#include "uchat.h"

t_list* chat_parse_from_json_arr(cJSON* arr) {
    if (arr == NULL || !cJSON_IsArray(arr)) {
        return NULL;
    }

    t_list* list = NULL;

    for(int i = 0 ; i < cJSON_GetArraySize(arr); i++) {
        t_chat* chat = chat_parse_from_json(cJSON_GetArrayItem(arr, i));
        
        if (chat != NULL) {
            mx_push_back(&list, chat);
        } 
    }

    return list;
}

