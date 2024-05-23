#include "templates/imagemessage.h"
#include <uchat.h>

struct _UchatImageMessage {
    GtkWidget parent_instance;

    GtkWidget* avatar;
    GtkWidget* image;
    GtkWidget* time;
};

G_DEFINE_TYPE(UchatImageMessage, uchat_image_message, GTK_TYPE_WIDGET)

static void
uchat_image_message_class_init(UchatImageMessageClass *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

    gtk_widget_class_set_template_from_resource(widget_class, "/ua/ucode-connect/uchat/ui/imagemessage.ui");
    gtk_widget_class_set_layout_manager_type(widget_class, GTK_TYPE_BOX_LAYOUT);

    gtk_widget_class_bind_template_child(widget_class, UchatImageMessage, avatar);
    gtk_widget_class_bind_template_child(widget_class, UchatImageMessage, image);
    gtk_widget_class_bind_template_child(widget_class, UchatImageMessage, time);
}

void
uchat_image_message_set_image(UchatImageMessage* self, const gchar* path) {
    gtk_picture_set_filename(GTK_PICTURE(self->image),path);
}

GFile*
uchat_image_message_get_image(UchatImageMessage* self) {
    return gtk_picture_get_file(GTK_PICTURE(self->image));
}

void
uchat_image_message_set_time(UchatImageMessage* self, const gchar* time) {
    gtk_label_set_label(GTK_LABEL(self->time), time);
}

const gchar *
uchat_image_box_get_timee(UchatImageMessage* self) {
    return gtk_label_get_label(GTK_LABEL(self->time));
}

static void
uchat_image_message_init(UchatImageMessage *self) {
    gtk_widget_init_template(GTK_WIDGET(self));
}

UchatImageMessage*
uchat_image_message_new(t_message* message, bool own) {
    UchatImageMessage* obj = g_object_new(UCHAT_TYPE_IMAGE_MESSAGE, NULL);

    gtk_widget_set_visible(obj->avatar, !own);
    gtk_widget_set_halign(GTK_WIDGET(obj), own ? GTK_ALIGN_END : GTK_ALIGN_START);
    gtk_widget_set_halign(GTK_WIDGET(obj->image), own ? GTK_ALIGN_END : GTK_ALIGN_START);

    cJSON* request = NULL;
    cJSON* response = NULL;
    cJSON* data = NULL;
    cJSON* headers = NULL;
    const char* path = NULL;

    headers = cJSON_CreateObject();
    cJSON_AddStringToObject(headers, "Authorization", uchat->user->session);

    data = cJSON_CreateObject();
    request = create_request(METHOD_GET, mx_strjoin("/messages/",mx_itoa(message->chat_id)), data, headers);

    int status = send_request(uchat->servsock, request);

    if (status != REQUEST_SUCCESS) {
        handle_error(REQUEST_ERROR, "\'GET /messages\'");
    }
    response = g_async_queue_pop(uchat->responses);

    if (cJSON_HasObjectItem(response, "status")) {
        status = cJSON_GetObjectItemCaseSensitive(response, "status")->valueint;
        if (status == 200) {
            cJSON* response_data = cJSON_GetObjectItemCaseSensitive(response, "data");
            for(int i = 0; i < cJSON_GetArraySize(response_data); i++){
                if(cJSON_GetObjectItemCaseSensitive(response_data, "id")->valueint == message->id){
                    unsigned long size;
                    char* image = strdup(cJSON_GetObjectItemCaseSensitive(response_data, "content")->valuestring);
                    unsigned char* from_bytes = g_base64_decode(image,&size);
                    char* dir = mx_strjoin("storage/chat", mx_itoa(message->chat_id));
                    int result = mkdir(dir, 0777);
                    path = mx_strjoin(dir, mx_strjoin("/",mx_itoa(message->id)));
                    bytes_to_file(from_bytes,size,path);
                }
            }
        }
        cJSON_Delete(response);
    }
    else {
        handle_error(RESPONSE_ERROR, "POST /messages");
    }
    uchat_image_message_set_image(obj,path);
    uchat_image_message_set_time(obj, strndup(&(message->time[11]), 5));

    return obj;
}
