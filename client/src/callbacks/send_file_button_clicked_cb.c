#include "uchat.h"

static void on_open_responsed(GtkDialog *dialog, int response, gpointer user_data){
    GtkNotebook* chats = GTK_NOTEBOOK(gtk_builder_get_object(uchat->builder, "message-container"));
    int id = gtk_notebook_get_current_page(chats);
    UchatMessageBox* chat = UCHAT_MESSAGE_BOX(gtk_notebook_get_nth_page(chats, id));
    
    if (response == GTK_RESPONSE_ACCEPT) {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);

        GtkFileFilter *filter = gtk_file_filter_new();
        gtk_file_filter_set_name(filter, "Image");
        gtk_file_filter_add_mime_type(filter, "image/png");

        gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);
        g_autoptr(GFile) file = gtk_file_chooser_get_file (chooser);
        const gchar *path = g_file_get_path(file);
        long size;
        unsigned char* p = file_to_bytes(path, &size);
        char* encode = g_base64_encode(p, size);

        cJSON* request = NULL;
        cJSON* response = NULL;
        cJSON* data = NULL;
        cJSON* headers = NULL;

        headers = cJSON_CreateObject();
        cJSON_AddStringToObject(headers, "Authorization", uchat->user->session);

        data = cJSON_CreateObject();
        cJSON_AddNumberToObject(data, "chat_id", uchat->user->current_chat->id);
        cJSON_AddStringToObject(data, "type", "photo");
        cJSON_AddStringToObject(data, "content", encode);

        request = create_request(METHOD_POST, "/messages", data, headers);

        int status = send_request(uchat->servsock, request);

        if (status != REQUEST_SUCCESS) {
            handle_error(REQUEST_ERROR, "POST /messages");
        }
        
        response = g_async_queue_pop(uchat->responses);
        
        if (cJSON_HasObjectItem(response, "status")) {
            status = cJSON_GetObjectItemCaseSensitive(response, "status")->valueint;

            if (status == 201) {
                cJSON* response_data = cJSON_GetObjectItemCaseSensitive(response, "data");
                t_message* message = message_parse_from_json(response_data);
                uchat_message_box_add_image(chat, message, true);
            }

            cJSON_Delete(response);
        }
        else {
            handle_error(RESPONSE_ERROR, "POST /messages");
        }

        g_free((gpointer)path);
    }

    gtk_window_destroy(GTK_WINDOW (dialog));
}

void send_file_button_clicked_cb(GtkButton* self, gpointer user_data) {
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(uchat->builder, "main-window"));
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;

    dialog = gtk_file_chooser_dialog_new ("Open File",
                                      window,
                                      action,
                                      ("_Cancel"),
                                      GTK_RESPONSE_CANCEL,
                                      ("_Open"),
                                      GTK_RESPONSE_ACCEPT,
                                      NULL);

    g_signal_connect(dialog, "response", G_CALLBACK(on_open_responsed), user_data);
    gtk_widget_show(GTK_WIDGET(dialog));
}

    