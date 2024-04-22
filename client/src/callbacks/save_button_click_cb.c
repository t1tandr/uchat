#include "uchat.h"

void save_button_click_cb(GtkButton* self, gpointer user_data) {
    t_uchat_app* uchat = (t_uchat_app *)g_object_get_data(user_data, "uchat");
    const char* username = NULL;
    const char* password = NULL;
    const char* confirm_password = NULL;
    const char* biography = NULL;
    cJSON* request = NULL;
    cJSON* response = NULL;
    cJSON* data = NULL;
    cJSON* headers = NULL;


    username = gtk_editable_get_text(GTK_EDITABLE(gtk_builder_get_object(uchat->builder, "settings-get-username")));
    password = gtk_editable_get_text(GTK_EDITABLE(gtk_builder_get_object(uchat->builder, "settings-get-password")));
    confirm_password = gtk_editable_get_text(GTK_EDITABLE(gtk_builder_get_object(uchat->builder, "settings-get-password-confrim")));
    biography = gtk_editable_get_text(GTK_EDITABLE(gtk_builder_get_object(uchat->builder, "settings-get-biography")));

    if(username != NULL && password != NULL && confirm_password != NULL && biography != NULL) {   
        headers = cJSON_CreateObject();

        cJSON_AddStringToObject(headers, "Authorization", uchat->user->session);
    
        data = cJSON_CreateObject();
        cJSON_AddStringToObject(data, "username", username);
        cJSON_AddStringToObject(data, "bio", biography);

        int len = strlen(mx_itoa(uchat->user->id));
        char str[9 + len];
        sprintf(str,"/users/%d", uchat->user->id);

        request = create_request(METHOD_PUT, str, data, headers);

        mx_printstr(cJSON_Print(request));

        response = send_request(uchat->servsock, request);

        cJSON_Delete(request);

        GtkBox* homepage = GTK_BOX(gtk_builder_get_object(uchat->builder, "homepage"));
        GtkCenterBox* right_side = GTK_CENTER_BOX(gtk_builder_get_object(uchat->builder, "right-side"));
        GtkScrolledWindow* settings = GTK_SCROLLED_WINDOW(gtk_builder_get_object(uchat->builder, "settings"));

        gtk_box_remove(homepage, GTK_WIDGET(settings));
        gtk_box_append(homepage, GTK_WIDGET(right_side));
        cJSON_Delete(response);

    }
}