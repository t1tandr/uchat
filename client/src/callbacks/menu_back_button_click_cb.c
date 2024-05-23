#include "uchat.h"

void menu_back_button_click_cb(GtkButton* self, gpointer user_data) {
    GtkWidget* box = GTK_WIDGET(gtk_builder_get_object(uchat->builder, "sidebar-box"));

    gtk_widget_set_visible(GTK_WIDGET(box), FALSE);
}

