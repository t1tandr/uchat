#include "../inc/libmx.h"

void mx_push_back(t_list **list, void *data) {
	if (*list == NULL || list == NULL) {
		printf("%p\n", data);
		*list = mx_create_node(data);
		return;
	}

	printf("%p\n", data);
    	
   	t_list *temp = *list;
    while (temp->next) { 
        temp = temp->next;
    }
    temp->next = mx_create_node(data);
}


