#include "uchat.h"

void settings_button_click_cb(GtkButton* self, gpointer user_data) {
    GtkBox* homepage = GTK_BOX(gtk_builder_get_object(uchat->builder, "homepage"));
    GtkNotebook* right_side = GTK_NOTEBOOK(gtk_builder_get_object(uchat->builder, "message-container"));
    GtkScrolledWindow* settings = GTK_SCROLLED_WINDOW(gtk_builder_get_object(uchat->builder, "settings"));
    GtkEntry* username = GTK_ENTRY(gtk_builder_get_object(uchat->builder, "settings-get-username"));
    GtkEntry* name = GTK_ENTRY(gtk_builder_get_object(uchat->builder, "settings-get-name"));
    GtkTextView* bio = GTK_TEXT_VIEW(gtk_builder_get_object(uchat->builder, "settings-get-biography"));
    GtkEditable* password_entry = GTK_EDITABLE(gtk_builder_get_object(uchat->builder, "settings-get-password"));
    GtkEditable* confirm_entry = GTK_EDITABLE(gtk_builder_get_object(uchat->builder, "settings-get-password-confrim"));
    GtkRevealer* is_taken_revealer = GTK_REVEALER(gtk_builder_get_object(uchat->builder, "username-is-taken-revealer-in-settings"));
    GtkRevealer* unmatch_revealer = GTK_REVEALER(gtk_builder_get_object(uchat->builder, "password-unmatch-revealer-in-settings"));
    
    gtk_entry_set_placeholder_text(username,uchat->user->username);
    gtk_entry_set_placeholder_text(name,uchat->user->name);
    gtk_editable_set_text(password_entry, "");
    gtk_editable_set_text(confirm_entry, "");
    gtk_revealer_set_reveal_child(is_taken_revealer, FALSE);
    gtk_revealer_set_reveal_child(unmatch_revealer, FALSE);

    // if(uchat->user->bio != NULL){
    //     GtkTextBuffer* buffer = gtk_text_buffer_new(gtk_text_tag_table_new());
    //     gtk_text_buffer_set_text (buffer,uchat->user->bio,strlen(uchat->user->bio)+1);
    //     gtk_text_view_set_buffer (bio,buffer);
    // }

    gtk_box_remove(homepage, GTK_WIDGET(right_side));
    gtk_box_append(homepage, GTK_WIDGET(settings));
}

