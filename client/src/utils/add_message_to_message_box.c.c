#include "uchat.h"

void add_message_to_message_box(t_message* message) {
    GtkNotebook* notebook = GTK_NOTEBOOK(gtk_builder_get_object(uchat->builder, "message-container"));
    int n_pages = gtk_notebook_get_n_pages(notebook);

    for(int i = 1; i < n_pages; i++) {
        UchatMessageBox* page = UCHAT_MESSAGE_BOX(gtk_notebook_get_nth_page(notebook, i));
        
        if (message->chat_id == uchat_message_box_get_chat(page)->id) {
            uchat_message_box_add_message(page, message, message->user_id == uchat->user->id);
            return;
        }
    }

    // UchatMessageBox* box = uchat_message_box_new(chat);
    // gtk_notebook_append_page(notebook, GTK_WIDGET(box), gtk_label_new(chat->name));
    // gtk_notebook_set_current_page(notebook, -1);
    // uchat->user->current_chat = chat;
}

