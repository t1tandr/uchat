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
    t_uchat* uchat = (t_uchat *)g_object_get_data(user_data, "uchat");
    GtkTextView* view = GTK_TEXT_VIEW(gtk_builder_get_object(uchat->builder, "message-entry"));
    GtkScrolledWindow* window = GTK_SCROLLED_WINDOW(gtk_builder_get_object(uchat->builder, "message-container-scrolled"));
    GtkAdjustment* vadj = NULL;

    gchar* text = text_view_get_full_text(view);

    if (strlen(mx_strtrim(text)) > 0) {
        cJSON* request = NULL;
        cJSON* response = NULL;
        cJSON* data = NULL;
        cJSON* headers = NULL;

        headers = cJSON_CreateObject();
        cJSON_AddStringToObject(headers, "Authorization", uchat->session);

        data = cJSON_CreateObject();
        cJSON_AddNumberToObject(data, "chat_id", uchat->user/*->current_chat*/->id);
        cJSON_AddStringToObject(data, "text", text);

        request = create_request(METHOD_POST, "/message", data, headers);

        response = send_request(uchat->servsock, request);

        if (response != NULL && cJSON_HasObjectItem(response, "status") && cJSON_HasObjectItem(response, "data")) {
            int status = cJSON_GetObjectItemCaseSensitive(response, "status")->valueint;

            if (status == 201) {
                time_t current_time = time(NULL);
                GtkWidget* message = GTK_WIDGET(uchat_message_box_new(text, localtime(&current_time)));
                GtkWidget* message_container = GTK_WIDGET(gtk_builder_get_object(uchat->builder, "message-container"));

                gtk_widget_insert_before(message, message_container, NULL);
            }

            cJSON_Delete(response);
        }
        else {
            handle_error("uchat: error getting response from server");
        }

        /*
        vadj = gtk_scrolled_window_get_vadjustment(window);

        g_print("lower - %f\n", gtk_adjustment_get_lower(vadj));
        g_print("upper - %f\n", gtk_adjustment_get_upper(vadj));
        g_print("page size - %f\n", gtk_adjustment_get_page_size(vadj));
        g_print("value - %f\n", gtk_adjustment_get_value(vadj));

        gtk_adjustment_set_value(vadj, gtk_adjustment_get_upper(vadj) - gtk_adjustment_get_page_size(vadj));

        g_print("new value - %f\n\n", gtk_adjustment_get_value(vadj));
        */
    }

    g_free((gpointer)text);
}