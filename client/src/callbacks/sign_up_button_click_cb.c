#include "uchat.h"

void sign_up_button_click_cb(GtkButton* self, gpointer user_data) {
    GtkBuilder* builder = GTK_BUILDER(user_data);
    GtkBox* wrapper = GTK_BOX(gtk_builder_get_object(builder, "form"));
    GtkBox* sign_in_form = GTK_BOX(gtk_builder_get_object(builder, "sign-in"));
    GtkBox* sign_up_form = GTK_BOX(gtk_builder_get_object(builder, "sign-up"));

    gtk_box_remove(wrapper, GTK_WIDGET(sign_in_form));
    gtk_box_append(wrapper, GTK_WIDGET(sign_up_form));
}

