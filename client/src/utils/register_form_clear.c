#include "uchat.h"

void register_form_clear(GtkBuilder* builder) {
    GtkRevealer* password_reqs_revealer = GTK_REVEALER(gtk_builder_get_object(builder, "password-reqs-revealer"));
    GtkRevealer* is_taken_revealer = GTK_REVEALER(gtk_builder_get_object(builder, "username-is-taken-revealer"));
    GtkEditable* username_entry = GTK_EDITABLE(gtk_builder_get_object(builder, "register-username"));
    GtkEditable* name_entry = GTK_EDITABLE(gtk_builder_get_object(builder, "register-name"));
    GtkEditable* password_entry = GTK_EDITABLE(gtk_builder_get_object(builder, "register-password"));
    GtkEditable* confirm_entry = GTK_EDITABLE(gtk_builder_get_object(builder, "register-password-confirm"));

    gtk_revealer_set_reveal_child(is_taken_revealer, FALSE);
    gtk_revealer_set_reveal_child(password_reqs_revealer, FALSE);
    gtk_editable_set_text(username_entry, "");
    gtk_editable_set_text(name_entry, "");
    gtk_editable_set_text(password_entry, "");
    gtk_editable_set_text(confirm_entry, "");
}

