#include "uchat.h"

void quit_button_click_cb(GtkButton* self, gpointer user_data) {
    GtkBuilder* builder = GTK_BUILDER(user_data);
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "main-window"));
    gtk_window_destroy(GTK_WINDOW(window));
}

void settings_button_click_cb(GtkButton* self, gpointer user_data) {
    GtkBuilder* builder = GTK_BUILDER(user_data);
    GtkBox* homepage = GTK_BOX(gtk_builder_get_object(builder, "home-page"));
    GtkBox* right_side_menu = GTK_BOX(gtk_builder_get_object(builder, "right-side-menu"));
    GtkBox* settings = GTK_BOX(gtk_builder_get_object(builder, "settings"));

    gtk_box_remove(homepage, GTK_WIDGET(right_side_menu));
    gtk_box_append(homepage, GTK_WIDGET(settings));
}

void return_from_settings_button_click_cb(GtkButton* self, gpointer user_data) {
    GtkBuilder* builder = GTK_BUILDER(user_data);
    GtkBox* homepage = GTK_BOX(gtk_builder_get_object(builder, "home-page"));
    GtkBox* right_side_menu = GTK_BOX(gtk_builder_get_object(builder, "right-side-menu"));
    GtkBox* settings = GTK_BOX(gtk_builder_get_object(builder, "settings"));
    // GtkColorChooserWidget* chooser = GTK_COLOR_CHOOSER_WIDGET(gtk_builder_get_object(builder, "ColorChooser"));

    // g_object_set(chooser,"show-editor",FALSE,NULL);

    gtk_box_remove(homepage, GTK_WIDGET(settings));
    gtk_box_append(homepage, GTK_WIDGET(right_side_menu));
}

// void change_box_color(GtkColorChooserWidget *chooser, GdkRGBA *color, gpointer user_data) {
//     GtkBuilder* builder = GTK_BUILDER(user_data);
//     GtkWidget* box = GTK_WIDGET(gtk_builder_get_object(builder, "BoxForImage"));
//     char *css = g_strdup_printf("box { background-color: %s;border-radius: 50%%; }",gdk_rgba_to_string(color));

//     GtkCssProvider *provider = gtk_css_provider_new();
//     gtk_css_provider_load_from_data(provider, css, -1);
//     gtk_style_context_add_provider(gtk_widget_get_style_context(box),GTK_STYLE_PROVIDER(provider),GTK_STYLE_PROVIDER_PRIORITY_USER);
//     g_free(css);
// }

void on_response(GtkDialog *dialog, gint response_id, gpointer user_data) {
    if (response_id == GTK_RESPONSE_OK) {
        GdkRGBA color;
        gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(dialog),&color);
        GtkBuilder* builder = GTK_BUILDER(user_data);
        GtkWidget* box = GTK_WIDGET(gtk_builder_get_object(builder, "BoxForImage"));
        GtkWidget* box_1 = GTK_WIDGET(gtk_builder_get_object(builder, "BoxForImageinSettings"));
        char *css = g_strdup_printf("box { background-color: %s;border-radius: 50%%; }",gdk_rgba_to_string(&color));

        GtkCssProvider *provider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(provider, css, -1);
        gtk_style_context_add_provider(gtk_widget_get_style_context(box),GTK_STYLE_PROVIDER(provider),GTK_STYLE_PROVIDER_PRIORITY_USER);
        gtk_style_context_add_provider(gtk_widget_get_style_context(box_1),GTK_STYLE_PROVIDER(provider),GTK_STYLE_PROVIDER_PRIORITY_USER);
        g_free(css);
        gtk_widget_hide(GTK_WIDGET(dialog));
    } 
    else if (response_id == GTK_RESPONSE_CANCEL) {
        gtk_widget_hide(GTK_WIDGET(dialog));
    }
}

void open_but(GtkButton* self, gpointer user_data) {
    GtkBuilder* builder = GTK_BUILDER(user_data);
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "main-window"));
    
    GtkColorChooserDialog *dialog = GTK_COLOR_CHOOSER_DIALOG(gtk_color_chooser_dialog_new("Select Color", GTK_WINDOW(window)));
    gtk_window_set_transient_for(GTK_WINDOW(dialog),GTK_WINDOW(window));
    g_signal_connect(dialog, "response", G_CALLBACK(on_response), user_data);
    gtk_widget_show(GTK_WIDGET(dialog));
}



