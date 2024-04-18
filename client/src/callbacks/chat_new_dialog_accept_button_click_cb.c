#include "uchat.h"

void chat_new_dialog_accept_button_click_cb(GtkButton* self, gpointer user_data) {
    t_uchat_app* uchat = (t_uchat_app *)g_object_get_data(user_data, "uchat");
    GtkWidget* dialog = GTK_WIDGET(gtk_builder_get_object(uchat->builder, "chat-new-dialog"));

    gtk_widget_hide(dialog);
}
