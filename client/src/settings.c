#include "uchat.h"

void return_from_settings_button_click_cb(GtkButton* self, gpointer user_data) {
    GtkBox* homepage = GTK_BOX(gtk_builder_get_object(uchat->builder, "homepage"));
    GtkNotebook* right_side = GTK_NOTEBOOK(gtk_builder_get_object(uchat->builder, "message-container"));
    GtkScrolledWindow* settings = GTK_SCROLLED_WINDOW(gtk_builder_get_object(uchat->builder, "settings"));

    gtk_box_remove(homepage, GTK_WIDGET(settings));
    gtk_box_append(homepage, GTK_WIDGET(right_side));
}

void on_response(GtkDialog *dialog, gint response_id, gpointer user_data) {
    if (response_id == GTK_RESPONSE_OK) {
        GdkRGBA color;
        gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(dialog),&color);
        GtkWidget* box_1 = GTK_WIDGET(gtk_builder_get_object(uchat->builder, "box_for_image_in_settings"));
        char *css = g_strdup_printf("box { background-color: %s;border-radius: 50%%; }",gdk_rgba_to_string(&color));

        GtkCssProvider *provider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(provider, css, -1);
        gtk_style_context_add_provider(gtk_widget_get_style_context(box_1),GTK_STYLE_PROVIDER(provider),GTK_STYLE_PROVIDER_PRIORITY_USER);
        g_free(css);
        gtk_window_destroy(GTK_WINDOW (dialog));
    } 
    else if (response_id == GTK_RESPONSE_CANCEL) {
        gtk_window_destroy(GTK_WINDOW (dialog));
    }
}

void open_color_chooser(GtkButton* self, gpointer user_data) {
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(uchat->builder, "main-window"));
    
    GtkColorChooserDialog *dialog = GTK_COLOR_CHOOSER_DIALOG(gtk_color_chooser_dialog_new("Select Color", GTK_WINDOW(window)));
    gtk_window_set_transient_for(GTK_WINDOW(dialog),GTK_WINDOW(window));
    g_signal_connect(dialog, "response", G_CALLBACK(on_response), user_data);
    gtk_widget_show(GTK_WIDGET(dialog));
}
