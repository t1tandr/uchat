#include "uchat.h"

GtkBuilder* builder_create(const char* files[]) {
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


