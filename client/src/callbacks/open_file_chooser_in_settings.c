#include "uchat.h"

static void on_open_response (GtkDialog *dialog, int response, gpointer user_data) {
    t_user* user = (t_user *)malloc(sizeof(t_user));
    if (response == GTK_RESPONSE_ACCEPT)
        {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);

        GtkFileFilter *filter = gtk_file_filter_new();
        gtk_file_filter_set_name(filter, "Image");
        gtk_file_filter_add_mime_type(filter, "image/png");
        gtk_file_filter_add_mime_type(filter, "image/jpeg");

        gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

        g_autoptr(GFile) file = gtk_file_chooser_get_file (chooser);
        const gchar *path = g_file_get_path(file);
        long size;
        unsigned char* p = file_to_bytes(path, &size);
        char* encode = g_base64_encode(p, size);

        UchatAvatarBox *area = UCHAT_AVATAR_BOX(gtk_builder_get_object(uchat->builder, "settings-avatar"));
        uchat_avatar_box_set_file(area, file);

        UchatAvatarBox *area1 = UCHAT_AVATAR_BOX(gtk_builder_get_object(uchat->builder, "sidebar-avatar"));
        uchat_avatar_box_set_file(area1, file);

        cJSON* request = NULL;
        cJSON* response = NULL;
        cJSON* data = NULL;
        cJSON* headers = NULL;

        headers = cJSON_CreateObject();
        cJSON_AddStringToObject(headers, "Authorization", uchat->user->session);

        data = cJSON_CreateObject();
        cJSON_AddStringToObject(data, "avatar", encode);

        request = create_request(METHOD_PUT, mx_strjoin("/users/",mx_itoa(uchat->user->id)), data, headers);

        int status = send_request(uchat->servsock, request);

        if (status != REQUEST_SUCCESS) {
            handle_error(REQUEST_ERROR, "\'PUT /avatar\'");
        }
        
        response = g_async_queue_pop(uchat->responses);
        g_free((gpointer)path);

    }
    gtk_window_destroy(GTK_WINDOW (dialog));
}

void open_file_chooser(GtkButton* self, gpointer user_data) {
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

    gtk_window_set_transient_for(GTK_WINDOW(dialog),GTK_WINDOW(window));
    g_signal_connect(dialog, "response", G_CALLBACK(on_open_response), user_data);
    gtk_widget_show(GTK_WIDGET(dialog));
}