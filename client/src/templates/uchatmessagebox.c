#include "templates/uchatmessagebox.h"

struct _UchatMessageBox {
  GtkWidget parent_instance;

  GtkWidget* message;
  GtkWidget* time;
};

G_DEFINE_TYPE(UchatMessageBox, uchat_message_box, GTK_TYPE_WIDGET)

static void
uchat_message_box_class_init(UchatMessageBoxClass *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

    gtk_widget_class_set_template_from_resource(widget_class, "/ua/ucode-connect/uchat/ui/messagebox.ui");
    gtk_widget_class_set_layout_manager_type(widget_class, GTK_TYPE_BOX_LAYOUT);

    gtk_widget_class_bind_template_child(widget_class, UchatMessageBox, message);
    gtk_widget_class_bind_template_child(widget_class, UchatMessageBox, time);
}

/*

void 
uchat_message_box_set_name(UchatMessageBox* self, const char* name) {
    gtk_label_set_label(GTK_LABEL(self->name), name);
}

const gchar * 
uchat_chat_box_get_name(UchatChatBox* self) {
    return gtk_label_get_label(GTK_LABEL(self->name));
}

*/

void
uchat_message_box_set_message(UchatMessageBox* self, const gchar* message) {
    gtk_label_set_label(GTK_LABEL(self->message), message);
}

const gchar *
uchat_message_box_get_message(UchatMessageBox* self) {
    return gtk_label_get_label(GTK_LABEL(self->message));
}

void
uchat_message_box_set_time(UchatMessageBox* self, struct tm *time) {
    char timestr[6];

    strftime(timestr, sizeof timestr, "%H:%M", time);

    gtk_label_set_label(GTK_LABEL(self->time), timestr);
}

const char *
uchat_message_box_get_time(UchatMessageBox* self) {
    return gtk_label_get_label(GTK_LABEL(self->time));
}

/*

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

*/

static void
uchat_message_box_init(UchatMessageBox *self) {
    gtk_widget_init_template(GTK_WIDGET(self));
}

UchatMessageBox *
uchat_message_box_new(const char* message, struct tm* time) {
    UchatMessageBox* obj = g_object_new(UCHAT_TYPE_MESSAGE_BOX, NULL);

    uchat_message_box_set_message(obj, message);
    uchat_message_box_set_time(obj, time);

    return obj;
}