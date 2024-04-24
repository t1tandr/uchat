#include "uchat.h"

void message_box_realize_cb(UchatMessageBox* self, gpointer user_data) {
    t_chat* chat = uchat_message_box_get_chat(self);

    if (chat->messages != NULL) {
        for (t_list* i = chat->messages; i != NULL; i = i->next) {
            t_message* message = (t_message *)i->data;
            
            uchat_message_box_add_message(self, message, message->user_id == uchat->user->id);
        }
    }
}
