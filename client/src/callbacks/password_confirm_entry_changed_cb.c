#include "uchat.h"

void password_confirm_entry_changed_cb(GtkEntry* self, gpointer user_data) {
    GtkRevealer* revealer = GTK_REVEALER(gtk_builder_get_object(uchat->builder, "password-unmatch-revealer"));
    const char* password = gtk_editable_get_text(GTK_EDITABLE(gtk_builder_get_object(uchat->builder, "register-password")));
    const char* confirm = gtk_editable_get_text(GTK_EDITABLE(self));

    gtk_revealer_set_reveal_child(revealer, (strcmp(password, confirm) != 0) && strlen(confirm) != 0 && strlen(password) != 0);
}

void password_confirm_entry_changed_in_settings_cb(GtkEntry* self, gpointer user_data) {
    GtkRevealer* revealer = GTK_REVEALER(gtk_builder_get_object(uchat->builder, "password-unmatch-revealer-in-settings"));
    const char* password = gtk_editable_get_text(GTK_EDITABLE(gtk_builder_get_object(uchat->builder, "settings-get-password")));
    const char* confirm = gtk_editable_get_text(GTK_EDITABLE(self));

    gtk_revealer_set_reveal_child(revealer, (strcmp(password, confirm) != 0) && strlen(confirm) != 0 && strlen(password) != 0);
}

