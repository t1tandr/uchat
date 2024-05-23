#include "uchat.h"

void password_entry_changed_cb(GtkEntry* self, gpointer user_data) {
    GtkRevealer* revealer = GTK_REVEALER(gtk_builder_get_object(uchat->builder, "password-reqs-revealer"));
    const char* password = gtk_editable_get_text(GTK_EDITABLE(self));

    gtk_revealer_set_reveal_child(revealer, (!check_password_strength(password, uchat->builder)
                                            || gtk_revealer_get_child_revealed(revealer)) && strlen(password) != 0);
}
