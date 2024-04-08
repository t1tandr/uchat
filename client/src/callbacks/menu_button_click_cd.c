#include "uchat.h"

void menu_button_click_cb(GtkButton* self, gpointer user_data) {
    GtkBuilder* builder = GTK_BUILDER(user_data);
    GtkWidget* box = GTK_WIDGET(gtk_builder_get_object(builder, "sidebar-box"));

    gtk_widget_set_visible(GTK_WIDGET(box), TRUE);
}