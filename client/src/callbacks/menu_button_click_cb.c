#include "uchat.h"

void menu_button_click_cb(GtkButton* self, gpointer user_data) {
    t_uchat* uchat = (t_uchat *)g_object_get_data(user_data, "uchat");
    GtkWidget* box = GTK_WIDGET(gtk_builder_get_object(uchat->builder, "sidebar-box"));

    gtk_widget_set_visible(GTK_WIDGET(box), TRUE);
}