#include "uchat.h"

static void chat_new_dialog_response_cb(GtkDialog* self, int response_id, gpointer user_data) {
    if (response_id == GTK_RESPONSE_ACCEPT) {
        gtk_window_close(GTK_WINDOW(self));
    }
    else {
        gtk_window_close(GTK_WINDOW(self));
    }
}

static GtkWidget* create_chat_new_dialog(GObject* uchat_obj) {
    GtkWidget* dialog = gtk_window_new();
    gtk_window_set_default_size(GTK_WINDOW(dialog), 600, 550);
    gtk_window_set_transient_for(GTK_WINDOW(dialog), gtk_application_get_active_window(uchat->app));
    gtk_window_set_decorated(GTK_WINDOW(dialog), FALSE);
    gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);
    
    GtkWidget* content_area = gtk_center_box_new();
    gtk_orientable_set_orientation(GTK_ORIENTABLE(content_area), GTK_ORIENTATION_VERTICAL);
    gtk_widget_set_margin_bottom(content_area, 20);
    gtk_widget_set_margin_end(content_area, 20);
    gtk_widget_set_margin_start(content_area, 20);
    gtk_widget_set_margin_top(content_area, 20);

    GtkWidget* cancel_button = gtk_button_new_with_label("Cancel");
    GtkWidget* accept_button = gtk_button_new_with_label("Create");

    GtkWidget* start_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget* avatar_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_size_request(avatar_box, 100, 100);
    gtk_widget_add_css_class(avatar_box, "avatar");

    GtkWidget* name_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(name_entry), "Chat name...");

    gtk_box_append(GTK_BOX(start_box), avatar_box);
    gtk_box_append(GTK_BOX(start_box), name_entry);

    GtkWidget* button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(button_box), cancel_button);
    gtk_box_append(GTK_BOX(button_box), accept_button);

    GtkWidget* end_box = gtk_center_box_new();
    gtk_orientable_set_orientation(GTK_ORIENTABLE(end_box), GTK_ORIENTATION_HORIZONTAL);
    gtk_center_box_set_end_widget(GTK_CENTER_BOX(end_box), button_box);
    
    gtk_center_box_set_start_widget(GTK_CENTER_BOX(content_area), start_box);
    gtk_center_box_set_end_widget(GTK_CENTER_BOX(content_area), end_box);
    gtk_window_set_child(GTK_WINDOW(dialog), content_area);

    g_signal_connect(dialog, "destroy", G_CALLBACK(gtk_window_destroy), NULL);

    return dialog;
}

void chat_new_button_click_cb(GtkButton* self, gpointer user_data) {
    GtkWidget* dialog = GTK_WIDGET(gtk_builder_get_object(uchat->builder, "chat-new-dialog"));

    gtk_window_set_transient_for(GTK_WINDOW(dialog), gtk_application_get_active_window(uchat->app));
    gtk_widget_show(dialog);
}

