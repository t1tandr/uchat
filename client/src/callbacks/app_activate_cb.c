#include "uchat.h"

static GtkBuilder* setup_builder(const char* files[]) {
    GtkBuilder* builder = gtk_builder_new();
    GError* err = NULL;

    gtk_builder_set_current_object(builder, G_OBJECT(builder));

    for(int i = 0; files[i] != NULL; i++) {
        gtk_builder_add_from_file(builder, files[i], &err);

        if(err != NULL) {
            handle_error(mx_strjoin("uchat: failed to start application: ", err->message));
        }
    }

    return builder;
}

void app_activate_cb(GtkApplication *app) {
    const char* files[] = {
        "resources/ui/login.ui",
        "resources/ui/register.ui",
        "resources/ui/homepage.ui",
        NULL 
    };
    
    GtkBuilder* builder = setup_builder(files);
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "main-window"));

    add_css_stylesheet("resources/css/style.css");
    add_icon_theme("resources/icons");

    gtk_application_add_window(app, window);
    gtk_window_present(window);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_window_destroy), NULL);
}

