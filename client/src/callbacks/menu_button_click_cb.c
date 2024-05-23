#include "uchat.h"

void print_unsigned_char(unsigned char c) {
    // Attempt to write the character to standard output
    if (write(STDOUT_FILENO, &c, 1) != 1) {
        // If write() fails, print an error message
        perror("Error writing to standard output");
    }
}

void menu_button_click_cb(GtkButton* self, gpointer user_data) {
    GtkWidget* box = GTK_WIDGET(gtk_builder_get_object(uchat->builder, "sidebar-box"));

    gtk_widget_set_visible(GTK_WIDGET(box), TRUE);
    cJSON* request = NULL;
    cJSON* response = NULL;
    cJSON* data = NULL;
    cJSON* headers = NULL;
    const char* path = NULL;

    headers = cJSON_CreateObject();
    cJSON_AddStringToObject(headers, "Authorization", uchat->user->session);

    data = cJSON_CreateObject();
    request = create_request(METHOD_GET, mx_strjoin("/users/",mx_itoa(uchat->user->id)), data, headers);

    int status = send_request(uchat->servsock, request);

    if (status != REQUEST_SUCCESS) {
        handle_error(REQUEST_ERROR, "\'GET /users\'");
    }
    response = g_async_queue_pop(uchat->responses);

    if (cJSON_HasObjectItem(response, "status")) {
        status = cJSON_GetObjectItemCaseSensitive(response, "status")->valueint;
        if (status == 200) {
            cJSON* response_data = cJSON_GetObjectItemCaseSensitive(response, "data");
            
            if(cJSON_HasObjectItem(response_data, "avatar")){
                unsigned long size;
                char* image = strdup(cJSON_GetObjectItemCaseSensitive(response_data, "avatar")->valuestring);
                unsigned char* from_bytes = g_base64_decode(image, &size);
                path = mx_strjoin("storage/",mx_itoa(uchat->user->id));
                mx_printstr(path);
                bytes_to_file(from_bytes, size, path);

                UchatAvatarBox *area = UCHAT_AVATAR_BOX(gtk_builder_get_object(uchat->builder, "settings-avatar"));
                uchat_avatar_box_set_file(area, g_file_new_for_path(path));

                UchatAvatarBox *area1 = UCHAT_AVATAR_BOX(gtk_builder_get_object(uchat->builder, "sidebar-avatar"));
                uchat_avatar_box_set_file(area1, g_file_new_for_path(path));
            }

        }
        cJSON_Delete(response);
        g_free((gpointer)path);
    }
    else {
        handle_error(RESPONSE_ERROR, "POST /messages");
    }
}
