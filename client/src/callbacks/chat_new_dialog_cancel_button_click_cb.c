#include "uchat.h"

void chat_new_dialog_cancel_button_click_cb(GtkButton* self, gpointer user_data) {
    GtkWidget* dialog = GTK_WIDGET(gtk_builder_get_object(uchat->builder, "chat-new-dialog"));
    GtkWidget* chatname_entry = GTK_WIDGET(gtk_builder_get_object(uchat->builder, "chat-new-name-entry"));
    GtkWidget* search_entry = GTK_WIDGET(gtk_builder_get_object(uchat->builder, "chat-new-search-entry"));
    GtkWidget* list = GTK_WIDGET(gtk_builder_get_object(uchat->builder, "new-chat-members-list"));
    GtkWidget* child = NULL;

    gtk_editable_set_text(GTK_EDITABLE(chatname_entry), "");
    gtk_editable_set_text(GTK_EDITABLE(search_entry), "");
    
    while((child = gtk_widget_get_first_child(list)) != NULL) {
        gtk_widget_unparent(child);
        gtk_list_box_remove(GTK_LIST_BOX(list), child);
    }

    gtk_widget_hide(dialog);
}
