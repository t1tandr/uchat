#include "uchat.h"

void connect_css(const char* path) {
    GtkCssProvider* provider = gtk_css_provider_new();

    gtk_css_provider_load_from_path(provider, path);
    gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

