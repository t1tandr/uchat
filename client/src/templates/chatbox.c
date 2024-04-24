#include "uchat.h"

struct _UchatChatBox {
    GtkWidget parent_instance;

    t_chat* chat;
    GtkWidget* name;
    GtkWidget* message;
    GtkWidget* time;
    GtkWidget* delete;
};

G_DEFINE_TYPE(UchatChatBox, uchat_chat_box, GTK_TYPE_WIDGET)

static void
delete_button_clicked_cb(GtkButton* self, gpointer user_data) {
    t_chat* chat = (t_chat *)user_data;
}


static void
uchat_chat_box_class_init(UchatChatBoxClass *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

    gtk_widget_class_set_template_from_resource(widget_class, "/ua/ucode-connect/uchat/ui/chatbox.ui");
    gtk_widget_class_set_layout_manager_type(widget_class, GTK_TYPE_BOX_LAYOUT);

    gtk_widget_class_bind_template_child(widget_class, UchatChatBox, name);
    gtk_widget_class_bind_template_child(widget_class, UchatChatBox, message);
    gtk_widget_class_bind_template_child(widget_class, UchatChatBox, time);
    gtk_widget_class_bind_template_child(widget_class, UchatChatBox, delete);
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


