#include "uchat.h"

int servsock;

static void connect_css(const char* filename) {
    GtkCssProvider* provider = gtk_css_provider_new();

    gtk_css_provider_load_from_path(provider, filename);
    gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

static void add_icon_theme(const char* path) {
    GtkIconTheme* theme = gtk_icon_theme_get_for_display(gdk_display_get_default());
    
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

const char* entry_get_text(GtkEntry* self) {
    GtkEntryBuffer* entry_buf = gtk_entry_get_buffer(self);

    return gtk_entry_buffer_get_text(entry_buf);
}

void login_button_click_cb(GtkWidget *self, gpointer user_data) {
    GtkBuilder* builder = GTK_BUILDER(user_data);
    cJSON* data = NULL;
    cJSON* response = NULL;
    const char* username = NULL;
    const char* password = NULL;

    username = gtk_editable_get_text(GTK_EDITABLE(gtk_builder_get_object(builder, "username-entry")));
    password = gtk_editable_get_text(GTK_EDITABLE(gtk_builder_get_object(builder, "password-entry")));

    data = cJSON_CreateObject();

    cJSON_AddStringToObject(data, "username", username);
    cJSON_AddStringToObject(data, "password", password);

    response = send_request(servsock, create_request(METHOD_POST, "/login", data));

    if(response != NULL && cJSON_HasObjectItem(response, "status")) {
        if(cJSON_GetObjectItemCaseSensitive(response, "status")->valueint == 200) {
            printf("[CLIENT]: received message:\n%s\n", cJSON_Print(response));
        }
    }
    else {
        handle_error("uchat: invalid response JSON");
    }
}

void sign_up_button_click_cb(GtkButton* self, gpointer user_data) {
    GtkBuilder* builder = GTK_BUILDER(user_data);
    GtkBox* wrapper = GTK_BOX(gtk_builder_get_object(builder, "form"));
    GtkBox* sign_in_form = GTK_BOX(gtk_builder_get_object(builder, "sign-in"));
    GtkBox* sign_up_form = GTK_BOX(gtk_builder_get_object(builder, "sign-up"));

    gtk_box_remove(wrapper, GTK_WIDGET(sign_in_form));

    // gtk_widget_add_css_class(GTK_WIDGET(wrapper), "flipped");

    gtk_box_append(wrapper, GTK_WIDGET(sign_up_form));
}

void sign_in_back_button_click_cb(GtkButton* self, gpointer user_data) {
    GtkBuilder* builder = GTK_BUILDER(user_data);
    GtkBox* wrapper = GTK_BOX(gtk_builder_get_object(builder, "form"));
    GtkBox* sign_in_form = GTK_BOX(gtk_builder_get_object(builder, "sign-in"));
    GtkBox* sign_up_form = GTK_BOX(gtk_builder_get_object(builder, "sign-up"));

    gtk_box_remove(wrapper, GTK_WIDGET(sign_up_form));

    // gtk_widget_add_css_class(GTK_WIDGET(wrapper), "flipped");

    gtk_box_append(wrapper, GTK_WIDGET(sign_in_form));
}

void password_entry_changed_cb(GtkEntry* self, gpointer user_data) {
    GtkBuilder* builder = GTK_BUILDER(user_data);
    char* password = gtk_editable_get_chars(GTK_EDITABLE(self), 0, -1);
    bool strength = false;
    bool* reqs = check_password_strength(password, &strength);
    GtkWidget* box = GTK_WIDGET(gtk_builder_get_object(builder, "password-requirements"));

    gtk_widget_set_visible(box, !strength || gtk_widget_get_visible(box));

    for(int i = 0; i < PWD_NUM; i++) {
        check_password_requirement(reqs[i], builder, i);
    }
}

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
        GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));

        connect_css("resources/css/style.css");
        add_icon_theme("resources/icons");

        gtk_application_add_window(app, window);
        gtk_window_present(window);

        g_signal_connect(window, "destroy", G_CALLBACK(gtk_window_destroy), NULL);
    }
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

    return status;
}

