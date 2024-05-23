#include "uchat.h"

t_list* chat_member_parse_from_json_arr(cJSON* arr) {
    if (arr == NULL || !cJSON_IsArray(arr)) {
        return NULL;
    }

    t_list* list = NULL;

    for(int i = 0 ; i < cJSON_GetArraySize(arr); i++) {
        t_chat_member* member = chat_member_parse_from_json(cJSON_GetArrayItem(arr, i));
        
        if (member != NULL) {
            mx_push_back(&list, member);
        } 
    }

    return list;
}

