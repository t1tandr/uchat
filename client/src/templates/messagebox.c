#include "templates/messagebox.h"

struct _UchatMessageBox {
  GtkWidget parent_instance;
  
  t_chat* chat;
  GtkWidget* header;
  GtkWidget* textview;
  GtkWidget* container;
  GtkWidget* wrapper;
  GtkWidget* name;
  GtkWidget* num_of_members;
  GtkWidget* chooser;
};

G_DEFINE_TYPE(UchatMessageBox, uchat_message_box, GTK_TYPE_WIDGET)

void
scroll_value_changed (GtkAdjustment* self, gpointer user_data) {
    double value = gtk_adjustment_get_value(self);
    double upper = gtk_adjustment_get_upper(self);
    double page_size = gtk_adjustment_get_page_size(self);

    gtk_adjustment_set_value(self, upper - page_size);
}

static void 
emoji_chooser_picked_cb(GtkEmojiChooser* self, gchar* text, gpointer user_data) {
    GtkTextView* view = GTK_TEXT_VIEW(user_data);
    GtkTextBuffer* buffer = gtk_text_view_get_buffer(view);

    gtk_text_buffer_insert_at_cursor(buffer, text, strlen(text));
}

static gboolean 
gesture_released_cb(GtkGestureClick* self, gint n_press, gdouble x, gdouble y, gpointer user_data) {
    t_chat* chat = (t_chat *)user_data;
    
    GtkWindow* dialog = GTK_WINDOW(gtk_window_new());

    gtk_window_set_title(dialog, "Chat info");
    gtk_window_set_default_size(dialog, 350, 400);
    gtk_window_set_resizable(dialog, FALSE);
    gtk_window_set_transient_for(dialog, gtk_application_get_active_window(uchat->app));
    gtk_window_set_modal(dialog, TRUE);
    gtk_window_set_destroy_with_parent(dialog, TRUE);

    GtkWidget* content_area = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_start(content_area, 20);
    gtk_widget_set_margin_end(content_area, 20);
    gtk_widget_set_margin_top(content_area, 20);
    gtk_widget_set_margin_bottom(content_area, 20);

    GtkWidget* header = gtk_fixed_new();
    gtk_widget_set_margin_bottom(header, 20);

    char* label = mx_strjoin(mx_itoa(mx_list_size(chat->members)), " members");
    GtkWidget* members_num = gtk_label_new(label);
    gtk_widget_set_margin_top(members_num, 10);
    gtk_widget_add_css_class(members_num, "userbox-chat-description");
    GtkWidget* name = gtk_label_new(chat->name);
    gtk_widget_set_margin_top(name, 10);
    gtk_widget_add_css_class(name, "userbox-chat-name");
    GtkWidget* avatar = GTK_WIDGET(uchat_avatar_box_new(g_file_new_for_path("resources/img/logo.png"), 100));

    gtk_fixed_put(GTK_FIXED(header), avatar, 0, 0);
    gtk_fixed_put(GTK_FIXED(header), name, 120, 0);
    gtk_fixed_put(GTK_FIXED(header), members_num, 120, 30);

    GtkWidget* scrolled_window = gtk_scrolled_window_new();
    GtkWidget* members_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    for (t_list* i = chat->members; i != NULL; i = i->next) {
        cJSON* request = NULL;
        cJSON* response = NULL;
        cJSON* data = NULL;
        cJSON* headers = NULL;
        char route[128];
        sprintf(route, "/users/%d", ((t_chat_member *)i->data)->user_id);

        headers = cJSON_CreateObject();
        cJSON_AddStringToObject(headers, "Authorization", uchat->user->session);

        data = cJSON_CreateObject();

        request = create_request(METHOD_GET, route, data, headers);

        int status = send_request(uchat->servsock, request);

        if (status != REQUEST_SUCCESS) {
            handle_error(REQUEST_ERROR, "GET /users/{id}");
        }

        response = g_async_queue_pop(uchat->responses);
        
        if (cJSON_HasObjectItem(response, "status")) {
            status = cJSON_GetObjectItemCaseSensitive(response, "status")->valueint;

            if (status == 200) {
                cJSON* response_data = cJSON_GetObjectItemCaseSensitive(response, "data");
                t_user* user = user_parse_from_json(response_data);
                UchatUserBox* userbox = uchat_user_box_new(user);

                gtk_box_append(GTK_BOX(members_box), GTK_WIDGET(userbox));
            }

            cJSON_Delete(response);
        }
        else {
            handle_error(RESPONSE_ERROR, "GET /users/{id}");
        }
    }

    gtk_widget_set_size_request(scrolled_window, 310, 260);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), members_box);

    gtk_box_append(GTK_BOX(content_area), header);
    gtk_box_append(GTK_BOX(content_area), scrolled_window);

    gtk_window_set_child(dialog, content_area);

    gtk_window_present(dialog);

    return GDK_EVENT_STOP;
}

static void
uchat_message_box_class_init(UchatMessageBoxClass *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

    gtk_widget_class_set_template_from_resource(widget_class, "/ua/ucode-connect/uchat/ui/messagebox.ui");
    gtk_widget_class_set_layout_manager_type(widget_class, GTK_TYPE_BOX_LAYOUT);

    gtk_widget_class_bind_template_child(widget_class, UchatMessageBox, header);
    gtk_widget_class_bind_template_child(widget_class, UchatMessageBox, container);
    gtk_widget_class_bind_template_child(widget_class, UchatMessageBox, name);
    gtk_widget_class_bind_template_child(widget_class, UchatMessageBox, num_of_members);
    gtk_widget_class_bind_template_child(widget_class, UchatMessageBox, textview);
    gtk_widget_class_bind_template_child(widget_class, UchatMessageBox, header);
    gtk_widget_class_bind_template_child(widget_class, UchatMessageBox, chooser);
    gtk_widget_class_bind_template_child(widget_class, UchatMessageBox, wrapper);
}

gchar *
uchat_message_box_get_text(UchatMessageBox* self) {
    return text_view_get_full_text(GTK_TEXT_VIEW(self->textview));
}

t_chat *
uchat_message_box_get_chat(UchatMessageBox* self) {
    return self->chat;
}

void
uchat_message_box_set_num_of_members(UchatMessageBox* self, int n) {
    char* label = NULL;
    
    if (n > 1) {
        label = mx_strjoin(mx_itoa(n), " members");
    }
    else {
        label = mx_strjoin(mx_itoa(n), " members");
    }

    gtk_label_set_label(GTK_LABEL(self->num_of_members), label);
}

void
uchat_message_box_add_message(UchatMessageBox* self, t_message* message, bool own) {
    GtkWidget* msg = NULL;
    
    if (message->type == MSG_TYPE_TXT) {
        msg = GTK_WIDGET(uchat_text_message_new(message, own));
    }
    if (message->type == MSG_TYPE_IMG) {
        msg = GTK_WIDGET(uchat_image_message_new(message, own));
    }

    gtk_box_append(GTK_BOX(self->container), msg);
}

void
uchat_message_box_add_image(UchatMessageBox* self, t_message* message, bool own) {
    UchatImageMessage* msg = uchat_image_message_new(message, own);
    gtk_box_append(GTK_BOX(self->container), GTK_WIDGET(msg));
}

static void
uchat_message_box_init(UchatMessageBox *self) {
    gtk_widget_init_template(GTK_WIDGET(self));
}

UchatMessageBox *
uchat_message_box_new(t_chat* chat) {
    UchatMessageBox* obj = g_object_new(UCHAT_TYPE_MESSAGE_BOX, NULL);

    obj->chat = chat;
    gtk_label_set_label(GTK_LABEL(obj->name), chat->name);
    uchat_message_box_set_num_of_members(obj, mx_list_size(chat->members));

    GtkGesture* gesture = gtk_gesture_click_new();
    gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(gesture), GDK_BUTTON_PRIMARY);

    gtk_widget_add_controller(GTK_WIDGET(obj->header), GTK_EVENT_CONTROLLER(gesture));

    GtkAdjustment* vadj = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(obj->wrapper));
    g_signal_connect (vadj, "changed", G_CALLBACK(scroll_value_changed), NULL);
    g_signal_connect(gesture, "pressed", G_CALLBACK(gesture_released_cb), chat);
    g_signal_connect(obj->chooser, "emoji-picked", G_CALLBACK(emoji_chooser_picked_cb), obj->textview);

    return obj;
}

