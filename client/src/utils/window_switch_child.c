#include "uchat.h"

void window_switch_child(GtkBuilder* builder, char* prev_id, char* next_id) {
    GtkWidget* prev = GTK_WIDGET(gtk_builder_get_object(builder, prev_id));
    GtkWidget* next = GTK_WIDGET(gtk_builder_get_object(builder, next_id));
    GtkWindow* window = GTK_WINDOW(gtk_widget_get_parent(prev));

    gtk_widget_unparent(prev);
    gtk_window_set_child(window, next);
}

