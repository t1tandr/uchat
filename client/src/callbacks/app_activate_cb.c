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
        "resources/ui/homepage.ui",
        NULL 
    };
    GtkBuilder* builder = setup_builder(files);
    GtkWidget* window = GTK_WIDGET(gtk_application_window_new(app));
    
    gtk_window_set_title(GTK_WINDOW(window), "MonkeyChat!");
    gtk_window_set_default_size(GTK_WINDOW(window), 1920, 1080);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_window_set_child(GTK_WINDOW(window), GTK_WIDGET(gtk_builder_get_object(builder, "login-page")));

    add_css_stylesheet("resources/css/style.css");
    add_icon_theme("resources/icons");

    gtk_window_present(GTK_WINDOW(window));

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_window_destroy), NULL);
}

