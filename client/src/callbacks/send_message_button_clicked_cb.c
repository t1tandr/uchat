#include "uchat.h"

static gchar* text_view_get_full_text(GtkTextView* view) {
    GtkTextBuffer* buffer = gtk_text_view_get_buffer(view);
    GtkTextIter start, end;
    gchar* text = NULL;

    gtk_text_buffer_get_bounds(buffer, &start, &end);

    text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

    gtk_text_buffer_delete(buffer, &start, &end);

    return text;
}

void send_message_button_clicked_cb(GtkButton* self, gpointer user_data) {
    t_uchat_app* uchat = (t_uchat_app *)g_object_get_data(user_data, "uchat");
    GtkTextView* view = GTK_TEXT_VIEW(gtk_builder_get_object(uchat->builder, "message-entry"));

    gchar* text = text_view_get_full_text(view);

    if (strlen(mx_strtrim(text)) > 0) {
        time_t current_time = time(NULL);
        GtkWidget* message = GTK_WIDGET(uchat_message_box_new(text, localtime(&current_time)));
        GtkWidget* message_container = GTK_WIDGET(gtk_builder_get_object(uchat->builder, "message-container"));

        gtk_widget_insert_before(message, message_container, NULL);
    }

    g_free((gpointer)text);
}