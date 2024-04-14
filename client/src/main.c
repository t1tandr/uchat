#include "uchat.h"

int servsock;

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

static void app_activate_cb(GtkApplication *app, gpointer user_data) {
    const char* files[] = {
        "resources/ui/login.ui",
        "resources/ui/homepage.ui",
        NULL 
    };

    GObject* uchat_obj = g_object_new(G_TYPE_OBJECT, NULL);
    t_uchat_app* uchat = (t_uchat_app *)malloc(sizeof(t_uchat_app));
    uchat->servsock = servsock;
    uchat->builder = setup_builder(files, uchat_obj);
    g_object_set_data(uchat_obj, "uchat", uchat);

    g_print("%d\n", ((t_uchat_app *)g_object_get_data(uchat_obj, "uchat"))->servsock);

    GtkWidget* window = GTK_WIDGET(gtk_application_window_new(app));
    
    gtk_window_set_title(GTK_WINDOW(window), "MonkeyChat!");
    gtk_window_set_default_size(GTK_WINDOW(window), 1920, 1080);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_window_set_child(GTK_WINDOW(window), GTK_WIDGET(gtk_builder_get_object(uchat->builder, "login-page")));

    add_css_stylesheet("resources/css/style.css");
    add_icon_theme("resources/icons");

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

