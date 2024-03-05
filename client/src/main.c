#include "uchat.h"

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
    GtkWidget *dialog, *username_label, *password_label, *content_area;
    char username_buf[1024] = { 0 };
    char password_buf[1024] = { 0 };
    GtkBuilder *builder = GTK_BUILDER(data);

    gtk_button_set_label(GTK_BUTTON(button), "Sign in");

    dialog = gtk_dialog_new_with_buttons("Login", GTK_WINDOW(gtk_builder_get_object(builder, "window")), GTK_DIALOG_DESTROY_WITH_PARENT, "OK", GTK_RESPONSE_OK, NULL);
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
    gtk_window_set_default_size(GTK_WINDOW(dialog), 300, 200);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_widget_set_valign(content_area, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(content_area, GTK_ALIGN_CENTER);

    entry_get_formated_text(username_buf, "USERNAME: %s", GTK_ENTRY(gtk_builder_get_object(builder, "username-entry")));
    entry_get_formated_text(password_buf, "PASSWORD: %s", GTK_ENTRY(gtk_builder_get_object(builder, "password-entry")));

    username_label = gtk_label_new(username_buf);
    gtk_widget_set_valign(username_label, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(username_label, GTK_ALIGN_START);

    gtk_box_append(GTK_BOX(content_area), username_label);

    password_label = gtk_label_new(password_buf);
    gtk_widget_set_valign(password_label, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(password_label, GTK_ALIGN_START);

    gtk_box_append(GTK_BOX(content_area), password_label);

    gtk_window_set_child(GTK_WINDOW(dialog), content_area);
    gtk_window_present(GTK_WINDOW(dialog));
}

static void app_activate_cb(GtkApplication *app) {
    GError* err = NULL;
    GtkBuilder *builder = gtk_builder_new();
    
    gtk_builder_set_current_object(builder, G_OBJECT(builder));
    gtk_builder_add_from_file(builder, "ui/login.ui", &err);

    if(err != NULL) {
        fprintf(stderr, "ERROR: %s\n", err->message);
    }
    else {
        GtkWindow *window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));

        connect_css("css/style.css");
        add_icon_theme("./icons");

        gtk_application_add_window(app, window);
        gtk_window_present(window);
        g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_window_destroy), NULL);
    }
}

int main(int argc, char *argv[]) {
    GtkApplication *app = NULL;
    int status = 0;

    app = gtk_application_new("ua.ucode-connect.uchat", G_APPLICATION_FLAGS_NONE);

    g_signal_connect(app, "activate", G_CALLBACK(app_activate_cb), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);

    return status;
}

/*
<child>
                    <object class="GtkImage" id="image">
                        <property name="visible">True</property>
                        <property name="file">ui/logo.jpg</property>
                        <property name="pixel-size">240</property>
                    </object>
                </child>
    */

