#include "templates/chatbox.h"

struct _UchatChatBox {
    GtkWidget parent_instance;

    t_chat* chat;
    GtkWidget* name;
    GtkWidget* message;
    GtkWidget* time;
};

G_DEFINE_TYPE(UchatChatBox, uchat_chat_box, GTK_TYPE_WIDGET)

static gboolean gesture_released_cb(GtkGestureClick* self, gint n_press, gdouble x, gdouble y, gpointer user_data) {
    GtkWidget* menu = gtk_popover_new();
    GtkWidget* items = gtk_list_box_new();
    GtkWidget* delete = gtk_label_new("Leave chat");
    GdkRectangle rec = { x, y, 0 , 0 };
    gtk_list_box_append(GTK_LIST_BOX(items), delete);

    gtk_popover_set_position(GTK_POPOVER(menu), GTK_POS_RIGHT);
    //gtk_popover_set_pointing_to(GTK_POPOVER(menu), &rec);

    gtk_popover_set_child(GTK_POPOVER(menu), items);

    gtk_popover_popup(GTK_POPOVER(menu));

    return GDK_EVENT_STOP;
}

static void
uchat_chat_box_class_init(UchatChatBoxClass *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

    gtk_widget_class_set_template_from_resource(widget_class, "/ua/ucode-connect/uchat/ui/chatbox.ui");
    gtk_widget_class_set_layout_manager_type(widget_class, GTK_TYPE_BOX_LAYOUT);

    gtk_widget_class_bind_template_child(widget_class, UchatChatBox, name);
    gtk_widget_class_bind_template_child(widget_class, UchatChatBox, message);
    gtk_widget_class_bind_template_child(widget_class, UchatChatBox, time);
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
    if (message == NULL) {
        gtk_label_set_label(GTK_LABEL(self->message), "The chat is created");
    }
    else {
        char* content = NULL;

        if (message->type == MSG_TYPE_TXT) {
            content = mx_strjoin(message->author, mx_strjoin(": ", message->content));
        }
        else if (message->type == MSG_TYPE_IMG) {
            content = mx_strjoin(message->author, ": sends an image");
        }
        
        gtk_label_set_label(GTK_LABEL(self->message), content);
    }
}

const gchar *
uchat_chat_box_get_message(UchatChatBox* self) {
    return gtk_label_get_label(GTK_LABEL(self->message));
}

void
uchat_chat_box_set_time(UchatChatBox* self, const gchar* time) {
    gtk_label_set_label(GTK_LABEL(self->time), time);
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

    if (chat->last_message == NULL) {
        uchat_chat_box_set_time(obj, strndup(&(chat->created_at[11]), 5));
    }
    else {
        uchat_chat_box_set_time(obj, strndup(&(chat->last_message->time[11]), 5));
    }

    // GtkGesture* gesture = gtk_gesture_click_new();
    // gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(gesture), GDK_BUTTON_SECONDARY);
    // g_signal_connect(gesture, "pressed", G_CALLBACK(gesture_released_cb), obj);
    // gtk_widget_add_controller(GTK_WIDGET(obj), GTK_EVENT_CONTROLLER(gesture));

    return obj;
}

