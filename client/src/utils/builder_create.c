#include "uchat.h"

GtkBuilder* builder_create(const char* files[]) {
    GtkBuilder* builder = gtk_builder_new();
    GError* err = NULL;

    for(int i = 0; files[i] != NULL; i++) {
        gtk_builder_add_from_file(builder, files[i], &err);

        if(err != NULL) {
            handle_error(BUILDER_ERROR, err->message);
        }
    }

    return builder;
}


