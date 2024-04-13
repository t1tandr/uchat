#include "uchat.h"

void register_button_click_cb(GtkButton* self, gpointer user_data) {
    GtkBuilder* builder = GTK_BUILDER(user_data);
    GtkRevealer* revealer = GTK_REVEALER(gtk_builder_get_object(builder, "username-is-taken-revealer"));

    gtk_revealer_set_reveal_child(revealer, TRUE);

    GtkWidget* homepage = GTK_WIDGET(gtk_builder_get_object(builder, "homepage"));
    GtkWidget* login_page = GTK_WIDGET(gtk_builder_get_object(builder, "login-page"));
    GtkWindow* window = GTK_WINDOW(gtk_widget_get_parent(login_page));

    gtk_widget_unparent(login_page);
    gtk_window_set_child(window, homepage);
}

