#include "uchat.h"

void chat_search_entry_changed_cb(GtkSearchEntry* self, gpointer user_data) {
    t_uchat_app* uchat = (t_uchat_app *)g_object_get_data(user_data, "uchat");
    GtkListBox* box = GTK_LIST_BOX(gtk_builder_get_object(uchat->builder, "chat-list"));

    gtk_list_box_prepend(box, GTK_WIDGET(uchat_chat_box_new(gtk_editable_get_text(GTK_EDITABLE(self)))));

    // gtk_list_box_invalidate_filter(box);
}

