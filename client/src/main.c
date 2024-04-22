#include "uchat.h"

int servsock;
t_uchat* uchat;

void* listen_for_response(void* arg) {
    while (true) {
        cJSON* response = recv_response(uchat->servsock);

        if (response != NULL) {
            // TODO: something
        }
    }

    pthread_exit(NULL);
}

void init_listener_thread() {
    int status = 0;
    pthread_t id;

    status = pthread_create(&id, NULL, listen_for_response, NULL);
    if (status != 0) {
        handle_error(mx_strjoin("uchat: failed to create thread: ", strerror(errno)));
    }

    pthread_detach(id);
}

static void app_activate_cb(GtkApplication *app, gpointer user_data) {
    uchat = uchat_create(servsock, app);

    init_listener_thread();

    add_css_stylesheet("resources/css/style.css");
    add_icon_theme("resources/icons");

    GtkWidget* window = GTK_WIDGET(gtk_application_window_new(app));
    
    gtk_window_set_title(GTK_WINDOW(window), "MonkeyChat!");
    gtk_window_set_default_size(GTK_WINDOW(window), 1600, 900);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    
    char* session = mx_file_to_str("session.json");
    if(session == NULL) {
        if(errno != ENOENT) {
            handle_error(mx_strjoin("uchat: failed to get session: ", strerror(errno)));
        }
        gtk_window_set_child(GTK_WINDOW(window), GTK_WIDGET(gtk_builder_get_object(uchat->builder, "login-page")));
    }
    else {
        uchat->user = get_current_user_from_json(cJSON_Parse(session));
        gtk_window_set_child(GTK_WINDOW(window), GTK_WIDGET(gtk_builder_get_object(uchat->builder, "homepage")));
    }
    gtk_window_present(GTK_WINDOW(window));

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_window_destroy), NULL);
}

int main(int argc, char *argv[]) {
    if(argc != 3) {
        handle_error(USAGE_ERROR);
    }
    GtkApplication* app = NULL;
    int status = 0;

    servsock = connect_to_server(argv[1], argv[2]);

    app = gtk_application_new("ua.ucode-connect.uchat", G_APPLICATION_HANDLES_OPEN);

    g_signal_connect(app, "open", G_CALLBACK(app_activate_cb), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);
    close(servsock);

    return status;
}

