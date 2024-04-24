#include "uchat.h"

int servsock;
t_uchat* uchat;

static void app_activate_cb(GtkApplication *app, gpointer user_data) {
    uchat = uchat_create(servsock, app);

    init_listener_thread();

    add_css_stylesheet("resources/css/style.css");
    add_icon_theme("resources/icons");

    GtkWidget* window = GTK_WIDGET(gtk_application_window_new(app));
    
    gtk_window_set_title(GTK_WINDOW(window), "MonkeyChat!");
    gtk_window_set_default_size(GTK_WINDOW(window), 1200, 675);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    if(uchat->user == NULL) {
        gtk_window_set_child(GTK_WINDOW(window), GTK_WIDGET(gtk_builder_get_object(uchat->builder, "login-page")));
    }
    else {
        gtk_window_set_child(GTK_WINDOW(window), GTK_WIDGET(gtk_builder_get_object(uchat->builder, "homepage")));
    }
    gtk_window_present(GTK_WINDOW(window));

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_window_destroy), NULL);
}

int main(int argc, char *argv[]) {
    if(argc != 3) {
        handle_error(USAGE_ERROR, "");
    }
    GtkApplication* app = NULL;
    int status = 0;
    char appid[128];

    servsock = connect_to_server(argv[1], argv[2]);

    sprintf(appid, "ua.ucode-connect.uchat%d", getpid());

    app = gtk_application_new(appid, G_APPLICATION_HANDLES_OPEN);

    g_signal_connect(app, "open", G_CALLBACK(app_activate_cb), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);
    close(servsock);

    return status;
}

