#include "templates/uchatchatbox.h"

/*

enum {
  CHAT_BOX_NAME = 1,
  MAMAN_BAR_PAPA_NUMBER,
};

*/

struct _UchatChatBox {
  GtkWidget parent_instance;

  GtkWidget* name;
  GtkWidget* message;
  GtkWidget* time;
  GtkWidget* seen;
};

G_DEFINE_TYPE(UchatChatBox, uchat_chat_box, GTK_TYPE_WIDGET)

static void
uchat_chat_box_class_init(UchatChatBoxClass *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

    gtk_widget_class_set_template_from_resource(widget_class, "/ua/ucode-connect/uchat/ui/chatbox.ui");
    gtk_widget_class_set_layout_manager_type(widget_class, GTK_TYPE_BOX_LAYOUT);

    gtk_widget_class_bind_template_child(widget_class, UchatChatBox, name);
    gtk_widget_class_bind_template_child(widget_class, UchatChatBox, message);
    gtk_widget_class_bind_template_child(widget_class, UchatChatBox, time);
    gtk_widget_class_bind_template_child(widget_class, UchatChatBox, seen);
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
uchat_chat_box_set_message(UchatChatBox* self, const gchar* message) {
    gtk_label_set_label(GTK_LABEL(self->message), message);
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

void
uchat_chat_box_set_seen(UchatChatBox* self, gboolean seen) {
    if(seen) {
        gtk_label_set_label(GTK_LABEL(self->seen), "\\\\//");
    }
    else {
        gtk_label_set_label(GTK_LABEL(self->seen), "\\/");
    }
}

gboolean
uchat_chat_box_get_seen(UchatChatBox* self) {
    return strcmp(gtk_label_get_label(GTK_LABEL(self->seen)), "\\\\//") == 0;
}

static void
uchat_chat_box_init(UchatChatBox *self) {
    gtk_widget_init_template(GTK_WIDGET(self));
}

UchatChatBox *
uchat_chat_box_new(const char* name) {
    UchatChatBox* obj = g_object_new(UCHAT_TYPE_CHAT_BOX, NULL);

    uchat_chat_box_set_message(obj, name);

    return obj;
}