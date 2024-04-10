#include "uchat.h"

static gboolean filter_func(GtkListBoxRow* row, gpointer user_data) {
    UchatContactBox* msg = UCHAT_CONTACT_BOX(gtk_list_box_row_get_child(row));
    const char* text = gtk_editable_get_text(GTK_EDITABLE(user_data));

    return strncmp(uchat_contact_box_get_author(msg), text, strlen(text)) == 0;
}

void contacts_list_realize_cb(GtkListBox* self, gpointer user_data) {
    GtkBuilder* builder = GTK_BUILDER(user_data);
    GObject* search_entry = gtk_builder_get_object(builder, "contact-search");

    gtk_list_box_set_filter_func(self, filter_func, search_entry, NULL);
}

