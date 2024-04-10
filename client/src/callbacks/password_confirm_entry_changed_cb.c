#include "uchat.h"

void password_confirm_entry_changed_cb(GtkEntry* self, gpointer user_data) {
    GtkBuilder* builder = GTK_BUILDER(user_data);
    GtkRevealer* revealer = GTK_REVEALER(gtk_builder_get_object(builder, "password-unmatch-revealer"));
    const char* password = gtk_editable_get_text(GTK_EDITABLE(gtk_builder_get_object(builder, "register-password-entry")));
    const char* confirm = gtk_editable_get_text(GTK_EDITABLE(self));

    gtk_revealer_set_reveal_child(revealer, (strcmp(password, confirm) != 0) && strlen(confirm) != 0 );
}

