#include "uchat.h"

#include <string.h>

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

const char* entry_get_text(GtkEntry* entry) {
    GtkEntryBuffer *entry_buf = gtk_entry_get_buffer(entry);

    return gtk_entry_buffer_get_text(entry_buf);
}

void login_button_click_cb(GtkWidget *button, gpointer user_data) {
    if(button != NULL) {
        GtkBuilder *builder = NULL;
        cJSON *data = NULL;
        const char *username = NULL;
        const char *password = NULL;

        builder = GTK_BUILDER(user_data);

        username = entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "password-entry")));
        password = entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "username-entry")));

        data = cJSON_CreateObject();

        cJSON_AddStringToObject(data, "username", username);
        cJSON_AddStringToObject(data, "password", password);

        char* response = send_request(serv_sock, create_request(METHOD_POST, "user", data));

        printf("[CLIENT] - received message:\n%s\n", response);
    }
}

void app_activate_cb(GtkApplication *app) {
    GError* err = NULL;
    GtkBuilder *builder = NULL;
    
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "resources/ui/login.ui", &err);
    gtk_builder_set_current_object(builder, G_OBJECT(builder));

    if(err != NULL) {
        fprintf(stderr, "ERROR: %s\n", err->message);
    }
    else {
        GtkWindow *window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));

        connect_css("resources/css/style.css");
        add_icon_theme("resources/icons");

        gtk_application_add_window(app, window);
        gtk_window_present(window);

        g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_window_destroy), NULL);
    }
}

int main(int argc, char *argv[]) {
    if(argc != 3) {
        fprintf(stderr, "usage: uchat <server-ip> <server-port>\n");
        return EXIT_FAILURE;
    }

    GtkApplication *app = NULL;
    int status = 0;

    serv_sock = connect_to_server(argv[1], argv[2]);

    app = gtk_application_new("ua.ucode-connect.uchat", G_APPLICATION_HANDLES_OPEN);

    g_signal_connect(app, "open", G_CALLBACK(app_activate_cb), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);

    return status;
}

