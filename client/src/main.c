#include "uchat.h"

int servsock;

static const char* files[] = {
    "resources/ui/login.ui",
    "resources/ui/homepage.ui",
    "resources/ui/chatnewdialog.ui",
    NULL
};

static GtkBuilder* setup_builder(const char* files[], GObject* object) {
    GtkBuilder* builder = gtk_builder_new();
    GError* err = NULL;

    gtk_builder_set_current_object(builder, object);

    for(int i = 0; files[i] != NULL; i++) {
        gtk_builder_add_from_file(builder, files[i], &err);

        if(err != NULL) {
            handle_error(mx_strjoin("uchat: failed to start application: ", err->message));
        }
    }

    return builder;
}

static GObject* create_uchat_object(int sockfd, GtkApplication* app) {
    GObject* obj = g_object_new(G_TYPE_OBJECT, NULL);
    t_uchat_app* uchat = (t_uchat_app *)malloc(sizeof(t_uchat_app));

    uchat->servsock = sockfd;
    uchat->builder = setup_builder(files, obj);
    uchat->app = app;

    g_object_set_data(obj, "uchat", uchat);

    return obj;
}

static void app_activate_cb(GtkApplication *app, gpointer user_data) {
    GObject* uchat_obj = create_uchat_object(servsock, app);
    t_uchat_app* uchat = (t_uchat_app *)g_object_get_data(uchat_obj, "uchat");

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

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_window_destroy), NULL);

    gtk_window_present(GTK_WINDOW(window));
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

