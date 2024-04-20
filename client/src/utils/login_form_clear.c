#include "uchat.h"

void login_form_clear(GtkBuilder* builder) {
    GtkRevealer* login_error_revealer = GTK_REVEALER(gtk_builder_get_object(builder, "login-error-revealer"));
    GtkEditable* username_entry = GTK_EDITABLE(gtk_builder_get_object(builder, "login-username"));
    GtkEditable* password_entry = GTK_EDITABLE(gtk_builder_get_object(builder, "login-password"));

    gtk_revealer_set_reveal_child(login_error_revealer, FALSE);
    gtk_editable_set_text(username_entry, "");
    gtk_editable_set_text(password_entry, "");
}

