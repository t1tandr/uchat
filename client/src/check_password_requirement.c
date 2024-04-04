#include "uchat.h"

void check_password_requirement(bool req, GtkBuilder* builder, int id) {
    GtkWidget* label = GTK_WIDGET(gtk_builder_get_object(builder, req_fields[id][BUILDER_ID]));
    
    if(!req) {
        gtk_widget_add_css_class(label, "wrong-password");
        gtk_label_set_label(GTK_LABEL(label), req_fields[id][WRONG_LABEL]);
    }
    else {
        if(widget_has_css_class(label, "wrong-password")) {
            gtk_widget_remove_css_class(label, "wrong-password");
        }
        gtk_label_set_label(GTK_LABEL(label), req_fields[id][CORRECT_LABEL]);
    }
}

