#include "uchat.h"

void chat_list_row_selected_cb(GtkListBox* self, GtkListBoxRow* row, gpointer user_data) {
    UchatChatBox* chatbox = UCHAT_CHAT_BOX(gtk_list_box_row_get_child(row));
    GtkNotebook* notebook = GTK_NOTEBOOK(gtk_builder_get_object(uchat->builder, "message-container"));

    t_chat* chat = uchat_chat_box_get_chat(chatbox);
    int n_pages = gtk_notebook_get_n_pages(notebook);

    for(int i = 1; i < n_pages; i++) {
        GtkWidget* page = gtk_notebook_get_nth_page(notebook, i);
        
        if (chat->id == uchat_message_box_get_chat(UCHAT_MESSAGE_BOX(page))->id) {
            gtk_notebook_set_current_page(notebook, i);
            uchat->user->current_chat = chat;
            return;
        }
    }

    UchatMessageBox* box = uchat_message_box_new(chat);
    gtk_notebook_append_page(notebook, GTK_WIDGET(box), gtk_label_new(chat->name));
    gtk_notebook_set_current_page(notebook, -1);
    uchat->user->current_chat = chat;
}

