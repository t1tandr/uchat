#include "uchat.h"

void contact_search_changed_cb(GtkSearchEntry* self, gpointer user_data) {
    GtkBuilder* builder = GTK_BUILDER(user_data);
    GtkListBox* box = GTK_LIST_BOX(gtk_builder_get_object(builder, "contacts-list"));

    gtk_list_box_prepend(box, GTK_WIDGET(uchat_contact_box_new(gtk_editable_get_text(GTK_EDITABLE(self)))));

    // gtk_list_box_invalidate_filter(box);
}

