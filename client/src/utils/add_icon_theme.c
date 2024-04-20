#include "uchat.h"

void add_icon_theme(const char* path) {
    GtkIconTheme* theme = gtk_icon_theme_get_for_display(gdk_display_get_default());
    
    gtk_icon_theme_add_search_path(theme, path);
}

