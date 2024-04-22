#include "uchat.h"

void chat_search_entry_changed_cb(GtkSearchEntry* self, gpointer user_data) {
    GtkListBox* box = GTK_LIST_BOX(gtk_builder_get_object(uchat->builder, "chat-list"));

    gtk_list_box_invalidate_filter(box);
}

