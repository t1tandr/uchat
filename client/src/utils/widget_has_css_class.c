#include "uchat.h"

bool widget_has_css_class(GtkWidget* self, const char* class) {
    char** classes = gtk_widget_get_css_classes(self);

    for(int i = 0 ; classes[i] != NULL; i++) {
        if(strcmp(classes[i], class) == 0) {
            g_strfreev(classes);
            return true;
        }
    }

    g_strfreev(classes);
    return false;
}

