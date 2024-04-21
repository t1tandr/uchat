#include "uchat.h"

void log_out_button_click_cb(GtkWidget *self, gpointer user_data){
    t_uchat* uchat = (t_uchat *)g_object_get_data(user_data, "uchat");
    cJSON* request = NULL;
    cJSON* response = NULL;
    cJSON* data = NULL;
    cJSON* headers = NULL;

    headers = cJSON_CreateObject();

    data = cJSON_CreateObject();

    request = create_request(METHOD_POST, "/logout", data, headers);

    response = send_request(uchat->servsock, request);

    cJSON_Delete(request);
    cJSON_Delete(response);

    remove("session.json");

    window_switch_child(uchat->builder, "homepage", "login-page");

}