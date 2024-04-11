#include "uchat.h"

void switch_form_button_click_cb(GtkButton* self, gpointer user_data) {
    GtkBuilder* builder = GTK_BUILDER(user_data);
    GtkWidget* wrapper = GTK_WIDGET(gtk_builder_get_object(builder, "form"));
    GtkWidget* prev = gtk_widget_get_first_child(wrapper);
    GtkWidget* next = NULL;

    if(strcmp(gtk_buildable_get_buildable_id(GTK_BUILDABLE(prev)), "login") == 0) {
        next = GTK_WIDGET(gtk_builder_get_object(builder, "register"));
    }
    else {
        next = GTK_WIDGET(gtk_builder_get_object(builder, "login"));
    }

    gtk_box_remove(GTK_BOX(wrapper), prev);
    gtk_box_append(GTK_BOX(wrapper), next);
}

