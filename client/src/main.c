#include "uchat.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>

int connect_to_server(const char* ip, int port) {
    int sockfd = 0;
    struct sockaddr_in server_addr;

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr = (struct sockaddr_in) { 
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr.s_addr = inet_addr(ip)
    };

    if(connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    return sockfd;
}

int serv_sock;

static void connect_css(const char* filename) {
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, filename);
    gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

static void add_icon_theme(const char* path) {
    GtkIconTheme *theme = gtk_icon_theme_get_for_display(gdk_display_get_default());
    gtk_icon_theme_add_search_path(theme, path);
}

void password_entry_icon_press_cb(GtkEntry* self) {
    if(gtk_entry_get_visibility(self)) {
        gtk_entry_set_visibility(self, FALSE);
        gtk_entry_set_icon_from_icon_name(self, GTK_ENTRY_ICON_SECONDARY, "password-visibility-on");
    }
    else {
        gtk_entry_set_visibility(self, TRUE);
        gtk_entry_set_icon_from_icon_name(self, GTK_ENTRY_ICON_SECONDARY, "password-visibility-off");
    }
}

void entry_get_formated_text(char* buf, const char* format, GtkEntry* entry) {
    GtkEntryBuffer *entry_buf = gtk_entry_get_buffer(entry);
    sprintf(buf, format, gtk_entry_buffer_get_text(entry_buf));
}

void login_button_click_cb(GtkWidget *button, gpointer data) {
    if(button != NULL) {
        char username_buf[1024] = { 0 };
        char password_buf[1024] = { 0 };
        GtkBuilder *builder = GTK_BUILDER(data);

        entry_get_formated_text(username_buf, "USERNAME: %s", GTK_ENTRY(gtk_builder_get_object(builder, "username-entry")));
        entry_get_formated_text(password_buf, "PASSWORD: %s", GTK_ENTRY(gtk_builder_get_object(builder, "password-entry")));

        send(serv_sock, username_buf, sizeof username_buf, 0);
        send(serv_sock, password_buf, sizeof password_buf, 0);
    }
}

void app_activate_cb(GtkApplication *app) {
    GError* err = NULL;
    GtkBuilder *builder = gtk_builder_new();
    
    gtk_builder_set_current_object(builder, G_OBJECT(builder));
    gtk_builder_add_from_file(builder, "ui/login.ui", &err);

    if(err != NULL) {
        fprintf(stderr, "ERROR: %s\n", err->message);
    }
    else {
        GtkWindow *window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));

        connect_css("./css/style.css");
        add_icon_theme("./icons");

        gtk_application_add_window(app, window);
        gtk_window_present(window);
        g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_window_destroy), NULL);
    }
}

int main(int argc, char *argv[]) {
    if(argc != 3) {
        fprintf(stderr, "usage: uchat <server-ip> <port>\n");
        return EXIT_FAILURE;
    }

    GtkApplication *app = NULL;
    int status = 0;

    serv_sock = connect_to_server(argv[1], atoi(argv[2]));

    app = gtk_application_new("ua.ucode-connect.uchat", G_APPLICATION_HANDLES_OPEN);

    g_signal_connect(app, "open", G_CALLBACK(app_activate_cb), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);

    return status;
}

