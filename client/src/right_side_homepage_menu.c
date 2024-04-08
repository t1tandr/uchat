#include "uchat.h"

void quit_button_click_cb(GtkButton* self, gpointer user_data) {
    GtkBuilder* builder = GTK_BUILDER(user_data);
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "main-window"));
    gtk_window_destroy(GTK_WINDOW(window));
}

void settings_button_click_cb(GtkButton* self, gpointer user_data) {
    GtkBuilder* builder = GTK_BUILDER(user_data);
    GtkBox* homepage = GTK_BOX(gtk_builder_get_object(builder, "home-page"));
    GtkBox* right_side_menu = GTK_BOX(gtk_builder_get_object(builder, "right-side-menu"));
    GtkBox* settings = GTK_BOX(gtk_builder_get_object(builder, "settings"));

    gtk_box_remove(homepage, GTK_WIDGET(right_side_menu));
    gtk_box_append(homepage, GTK_WIDGET(settings));
}

void return_from_settings_button_click_cb(GtkButton* self, gpointer user_data) {
    GtkBuilder* builder = GTK_BUILDER(user_data);
    GtkBox* homepage = GTK_BOX(gtk_builder_get_object(builder, "home-page"));
    GtkBox* right_side_menu = GTK_BOX(gtk_builder_get_object(builder, "right-side-menu"));
    GtkBox* settings = GTK_BOX(gtk_builder_get_object(builder, "settings"));

    gtk_box_remove(homepage, GTK_WIDGET(settings));
    gtk_box_append(homepage, GTK_WIDGET(right_side_menu));
}