#include "uchat.h"

void switch_form_button_click_cb(GtkButton* self, gpointer user_data) {
    GtkRevealer* login_revealer = GTK_REVEALER(gtk_builder_get_object(uchat->builder, "login-revealer"));
    GtkRevealer* register_revealer = GTK_REVEALER(gtk_builder_get_object(uchat->builder, "register-revealer"));

    if(gtk_revealer_get_child_revealed(login_revealer)) {
        login_form_clear(uchat->builder);

        gtk_revealer_set_reveal_child(register_revealer, TRUE);
        gtk_revealer_set_reveal_child(login_revealer, FALSE);
    }
    else {
        register_form_clear(uchat->builder);

        gtk_revealer_set_reveal_child(register_revealer, FALSE);
        gtk_revealer_set_reveal_child(login_revealer, TRUE);
    }
}

