#include "uchat.h"

static void scroll_value_changed(GtkAdjustment *self, gpointer user_data) {
    GtkScrolledWindow* window = user_data;

    double value = gtk_adjustment_get_value(self);
    double upper = gtk_adjustment_get_upper(self);
    double page_size = gtk_adjustment_get_page_size(self);

    gtk_adjustment_set_value(self, upper - page_size - value);
}

static void scroll_bottom_gravity(GtkScrolledWindow *self, gpointer user_data) {
    GtkAdjustment *adjustment = gtk_scrolled_window_get_vadjustment(self);

    double upper = gtk_adjustment_get_upper(adjustment);
    double page_size = gtk_adjustment_get_page_size(adjustment);

    gtk_adjustment_set_value(adjustment, upper - page_size);
}

void message_container_scrolled_realize_cb(GtkScrolledWindow* self, gpointer user_data) {
    double from_bottom = 0.0;
    GtkAdjustment *adjustment = gtk_scrolled_window_get_vadjustment(self);

    g_signal_connect(adjustment, "value-changed", G_CALLBACK(scroll_value_changed), self);
}