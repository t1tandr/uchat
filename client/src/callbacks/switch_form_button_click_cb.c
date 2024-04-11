#include "uchat.h"

void switch_form_button_click_cb(GtkButton* self, gpointer user_data) {
    GtkBuilder* builder = GTK_BUILDER(user_data);
    GtkRevealer* login_revealer = GTK_REVEALER(gtk_builder_get_object(builder, "login-revealer"));
    GtkRevealer* register_revealer = GTK_REVEALER(gtk_builder_get_object(builder, "register-revealer"));

    if(gtk_revealer_get_child_revealed(login_revealer)) {
        gtk_revealer_set_reveal_child(register_revealer, TRUE);
        gtk_revealer_set_reveal_child(login_revealer, FALSE);
    }
    else {
        gtk_revealer_set_reveal_child(register_revealer, FALSE);
        gtk_revealer_set_reveal_child(login_revealer, TRUE);
    }
}

