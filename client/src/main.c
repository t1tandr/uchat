#include "uchat.h"

static const char* files[] = {
    "resources/ui/login.ui",
    "resources/ui/homepage.ui",
    "resources/ui/chatnewdialog.ui",
    NULL
};

struct args {
    int argc;
    char** argv;
};

int servsock;
t_uchat* uchat = NULL;

static GtkBuilder* setup_builder(const char* files[]) {
    GtkBuilder* builder = gtk_builder_new();
    GError* err = NULL;

    for(int i = 0; files[i] != NULL; i++) {
        gtk_builder_add_from_file(builder, files[i], &err);

        if(err != NULL) {
            handle_error(mx_strjoin("uchat: failed to start application: ", err->message));
        }
    }

    return builder;
}

static t_uchat* create_uchat_object(int sockfd, GtkApplication* app) {
    t_uchat* uchat = (t_uchat *)malloc(sizeof(t_uchat));

    uchat->servsock = sockfd;
    uchat->builder = setup_builder(files);
    uchat->app = app;
    uchat->user = NULL;

    return uchat;
}

static void app_activate_cb(GtkApplication *app, gpointer user_data) {
    uchat = create_uchat_object(servsock, app);

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
        cJSON* request = NULL;
        cJSON* response = NULL;
        cJSON* data = NULL;
        cJSON* headers = NULL;

        headers = cJSON_CreateObject();
        cJSON_AddStringToObject(headers, "Authorization", uchat->user->session);

        data = cJSON_CreateObject();

        request = create_request(METHOD_GET, "/chats", data, headers);

        response = send_request(uchat->servsock, request);

        if (response != NULL && cJSON_HasObjectItem(response, "status")) {
            int status = cJSON_GetObjectItemCaseSensitive(response, "status")->valueint;

            if (status == 201) {
                cJSON* response_data = cJSON_GetObjectItemCaseSensitive(response, "data");

                uchat->user->chats = get_chats_from_json_arr(response_data);
            }

            cJSON_Delete(response);
        }
        else {
            handle_error("uchat: error getting response from server");
        }

        gtk_window_set_child(GTK_WINDOW(window), GTK_WIDGET(gtk_builder_get_object(uchat->builder, "homepage")));
    }

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_window_destroy), NULL);

    gtk_window_present(GTK_WINDOW(window));
}

void* start_app(void* arg) {
    struct args args = *((struct args *)arg);
    GtkApplication* app = NULL;
    int status = 0;

    servsock = connect_to_server(args.argv[1], args.argv[2]);

    app = gtk_application_new("ua.ucode-connect.uchat", G_APPLICATION_HANDLES_OPEN);

    g_signal_connect(app, "open", G_CALLBACK(app_activate_cb), NULL);

    status = g_application_run(G_APPLICATION(app), args.argc, args.argv);

    g_object_unref(app);

    close(servsock);

    pthread_exit(&status);
}

void* listen_for_response(void* arg) {
    printf("thread id = %d\n", *((int *)arg));

    pthread_exit(NULL);
}

int init_threads(int argc, char *argv[]) {
    int status = 0;
    pthread_t main_id, listener_id;
    struct args* args = (struct args *)malloc(sizeof(struct args));
    args->argc = argc;
    args->argv = argv;

    status = pthread_create(&main_id, NULL, start_app, args);
    if (status != 0) {
        handle_error(mx_strjoin("uchat: failed to create thread: ", strerror(errno)));
    }
    
    status = pthread_create(&listener_id, NULL, listen_for_response, &listener_id);
    if (status != 0) {
        handle_error(mx_strjoin("uchat: failed to create thread: ", strerror(errno)));
    }
    
    pthread_join(main_id, NULL);
    pthread_join(listener_id, NULL);

    return status;
}

int main(int argc, char *argv[]) {
    if(argc != 3) {
        handle_error(USAGE_ERROR);
    }

    return init_threads(argc, argv);;
}

