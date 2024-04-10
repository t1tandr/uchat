#include "uchat.h"

static gboolean filter_func(GtkListBoxRow* row, gpointer user_data) {
    GtkLabel* label = GTK_LABEL(gtk_list_box_row_get_child(row));
    GtkEditable* entry = GTK_EDITABLE(user_data);
    const char* text = gtk_editable_get_text(entry);

    return strncmp(gtk_label_get_label(label), text, strlen(text)) == 0;
}

void contacts_list_realize_cb(GtkListBox* self, gpointer user_data) {
    GtkBuilder* builder = GTK_BUILDER(user_data);
    GObject* search_entry = gtk_builder_get_object(builder, "contact-search");

    gtk_list_box_set_filter_func(self, filter_func, search_entry, NULL);
}

