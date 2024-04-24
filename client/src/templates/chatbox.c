#include "uchat.h"

struct _UchatChatBox {
    GtkWidget parent_instance;

    t_chat* chat;
    GtkWidget* name;
    GtkWidget* message;
    GtkWidget* time;
    // GtkWidget* delete;
};

G_DEFINE_TYPE(UchatChatBox, uchat_chat_box, GTK_TYPE_WIDGET)

bool 
cmp(void* a, void* b) {
    return ((t_chat *)a)->id == ((t_chat *)b)->id;
}

static void 
delete_chat_from_notebook(int id) {
    GtkNotebook* notebook = GTK_NOTEBOOK(gtk_builder_get_object(uchat->builder, "message-container"));
    int n_pages = gtk_notebook_get_n_pages(notebook);

    for(int i = 1; i < n_pages; i++) {
        GtkWidget* page = gtk_notebook_get_nth_page(notebook, i);
        
        if (id == uchat_message_box_get_chat(UCHAT_MESSAGE_BOX(page))->id) {
            if (gtk_notebook_get_current_page(notebook) == i) {
                gtk_notebook_set_current_page(notebook, 0);
            }

            gtk_notebook_remove_page(notebook, i);
            break;;
        }
    }
}

static void 
delete_chat_from_chat_list(int id) {
    GtkListBox* list = GTK_LIST_BOX(gtk_builder_get_object(uchat->builder, "chat-list"));
    GtkListBoxRow* row = NULL;
    guint j = 0;

    while ((row = gtk_list_box_get_row_at_index(list, j++)) != NULL) {
        UchatChatBox* box = UCHAT_CHAT_BOX(gtk_list_box_row_get_child(row));
        t_chat* chat = uchat_chat_box_get_chat(box);

        if (chat->id == id) {
            gtk_list_box_remove(list, GTK_WIDGET(row));
            break;
        }
    }
}

static void
delete_button_clicked_cb(GtkButton* self, gpointer user_data) {
    t_chat* chat = (t_chat *)user_data;

    for (t_list* i = chat->members; i != NULL; i = i->next) {
        t_chat_member* member = (t_chat_member *)i->data;

        if (member->user_id == uchat->user->id) {
            if (member->role == ROLE_ADMIN) {
                cJSON* request = NULL;
                cJSON* response = NULL;
                cJSON* data = NULL;
                cJSON* headers = NULL;
                char route[128];

                sprintf(route, "/chats/%d", member->chat_id);

                headers = cJSON_CreateObject();
                cJSON_AddStringToObject(headers, "Authorization", uchat->user->session);

                data = cJSON_CreateObject();

                request = create_request(METHOD_DELETE, route, data, headers);

                int status = send_request(uchat->servsock, request);

                if (status != REQUEST_SUCCESS) {
                    handle_error(REQUEST_ERROR, "DELETE /chats/{id}");
                }

                response = g_async_queue_pop(uchat->responses);
                
                if (cJSON_HasObjectItem(response, "status")) {
                    status = cJSON_GetObjectItemCaseSensitive(response, "status")->valueint;

                    if (status == 200) {
                        delete_chat_from_notebook(chat->id);
                        delete_chat_from_chat_list(chat->id);
                        // mx_del_node_if(&(uchat->user->chats), chat, cmp);
                    }

                    cJSON_Delete(response);
                }
                else {
                    handle_error(RESPONSE_ERROR, "DELETE /chats/{id}");
                }
            }
            else if (member->role == ROLE_USER) {
                cJSON* request = NULL;
                cJSON* response = NULL;
                cJSON* data = NULL;
                cJSON* headers = NULL;
                char route[128];

                sprintf(route, "/chat-members/%d", member->id);

                headers = cJSON_CreateObject();
                cJSON_AddStringToObject(headers, "Authorization", uchat->user->session);

                data = cJSON_CreateObject();

                request = create_request(METHOD_DELETE, route, data, headers);

                int status = send_request(uchat->servsock, request);

                if (status != REQUEST_SUCCESS) {
                    handle_error(REQUEST_ERROR, "DELETE /chat-members/{id}");
                }

                response = g_async_queue_pop(uchat->responses);
                
                if (cJSON_HasObjectItem(response, "status")) {
                    status = cJSON_GetObjectItemCaseSensitive(response, "status")->valueint;

                    if (status == 200) {
                        delete_chat_from_notebook(chat->id);
                        delete_chat_from_chat_list(chat->id);
                        // mx_del_node_if(&(uchat->user->chats), chat, cmp);
                    }

                    cJSON_Delete(response);
                }
                else {
                    handle_error(RESPONSE_ERROR, "DELETE /chat-members/{id}");
                }
            }
        }
    }
}


static void
uchat_chat_box_class_init(UchatChatBoxClass *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

    gtk_widget_class_set_template_from_resource(widget_class, "/ua/ucode-connect/uchat/ui/chatbox.ui");
    gtk_widget_class_set_layout_manager_type(widget_class, GTK_TYPE_BOX_LAYOUT);

    gtk_widget_class_bind_template_child(widget_class, UchatChatBox, name);
    gtk_widget_class_bind_template_child(widget_class, UchatChatBox, message);
    gtk_widget_class_bind_template_child(widget_class, UchatChatBox, time);
    // gtk_widget_class_bind_template_child(widget_class, UchatChatBox, delete);
}

void
uchat_chat_box_set_chat(UchatChatBox* self, t_chat* chat) {
    self->chat = chat;
}

t_chat *
uchat_chat_box_get_chat(UchatChatBox* self) {
    return self->chat;
}

void 
uchat_chat_box_set_name(UchatChatBox* self, const char* name) {
    gtk_label_set_label(GTK_LABEL(self->name), name);
}

const gchar * 
uchat_chat_box_get_name(UchatChatBox* self) {
    return gtk_label_get_label(GTK_LABEL(self->name));
}

void
uchat_chat_box_set_message(UchatChatBox* self, t_message* message) {
    char* label = NULL;
    char* content = NULL;
    const char* author = NULL;

    if (message == NULL) {
        uchat_chat_box_set_time(self, self->chat->created_at);
        gtk_label_set_label(GTK_LABEL(self->message), "Chat is created");
    }
    else {
        author = (message->user_id == uchat->user->id) ? "You" : message->author;

        if (message->type == MSG_TYPE_TXT) {
            if (strlen(message->content) > 15) {
                content = mx_strjoin(mx_strndup(message->content, 15), "...");
                label = mx_strjoin(author, mx_strjoin(": ", content));
            }
            else {
                label = mx_strjoin(author, mx_strjoin(": ", message->content));
            }
        }
        else if (message->type == MSG_TYPE_IMG) {
            label = mx_strjoin(author, ": sends an image");
        }
        
        gtk_label_set_label(GTK_LABEL(self->message), label);

        uchat_chat_box_set_time(self, message->time);
    }
}

const gchar *
uchat_chat_box_get_message(UchatChatBox* self) {
    return gtk_label_get_label(GTK_LABEL(self->message));
}

void
uchat_chat_box_set_time(UchatChatBox* self, const gchar* time) {
    gtk_label_set_label(GTK_LABEL(self->time), strndup(&(time[11]), 5));
}

const gchar *
uchat_chat_box_get_time(UchatChatBox* self) {
    return gtk_label_get_label(GTK_LABEL(self->time));
}

static void
uchat_chat_box_init(UchatChatBox *self) {
    gtk_widget_init_template(GTK_WIDGET(self));
}

UchatChatBox *
uchat_chat_box_new(t_chat* chat) {
    UchatChatBox* obj = g_object_new(UCHAT_TYPE_CHAT_BOX, NULL);

    obj->chat = chat;
    uchat_chat_box_set_name(obj, chat->name);
    uchat_chat_box_set_message(obj, chat->last_message);

    // g_signal_connect(obj->delete, "clicked", G_CALLBACK(delete_button_clicked_cb), chat);

    return obj;
}


