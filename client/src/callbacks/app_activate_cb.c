#include "uchat.h"

void app_activate_cb(GtkApplication *app) {
    GtkBuilder* builder = NULL;
    GError* err = NULL;
    
    builder = gtk_builder_new();
    gtk_builder_set_current_object(builder, G_OBJECT(builder));
    gtk_builder_add_from_file(builder, "resources/ui/login.ui", &err);

    if(err != NULL) {
        handle_error(mx_strjoin("uchat: failed to start application: ", err->message));
    }
    else {
        GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "main-window"));

        connect_css("resources/css/style.css");
        add_icon_theme("resources/icons");

        gtk_application_add_window(app, window);
        gtk_window_present(window);

        g_signal_connect(window, "destroy", G_CALLBACK(gtk_window_destroy), NULL);
    }
}

