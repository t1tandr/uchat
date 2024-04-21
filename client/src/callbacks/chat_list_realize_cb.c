#include "uchat.h"

static gboolean filter(GtkListBoxRow* row, gpointer user_data) {
    UchatChatBox* chat = UCHAT_CHAT_BOX(gtk_list_box_row_get_child(row));
    const char* text = gtk_editable_get_text(GTK_EDITABLE(user_data));

    return strncmp(uchat_chat_box_get_message(chat), text, strlen(text)) == 0;
}

void chat_list_realize_cb(GtkListBox* self, gpointer user_data) {
    t_uchat* uchat = (t_uchat *)g_object_get_data(user_data, "uchat");
    GObject* search_entry = gtk_builder_get_object(uchat->builder, "chat-search");

    gtk_list_box_set_filter_func(self, filter, search_entry, NULL);

    t_list* list = uchat->user->chats;
    while (list) {
        UchatChatBox* box = uchat_chat_box_new((t_chat *)list->data);
        gtk_list_box_append(self, GTK_WIDGET(box));

        list = list->next;
    }
}

