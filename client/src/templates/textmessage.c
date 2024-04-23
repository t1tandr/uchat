#include "templates/textmessage.h"

struct _UchatTextMessage {
    GtkWidget parent_instance;

    GtkWidget* avatar;
    GtkWidget* message;
    GtkWidget* time;
};

G_DEFINE_TYPE(UchatTextMessage, uchat_text_message, GTK_TYPE_WIDGET)

static void
uchat_text_message_class_init(UchatTextMessageClass *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

    gtk_widget_class_set_template_from_resource(widget_class, "/ua/ucode-connect/uchat/ui/textmessage.ui");
    gtk_widget_class_set_layout_manager_type(widget_class, GTK_TYPE_BOX_LAYOUT);

    gtk_widget_class_bind_template_child(widget_class, UchatTextMessage, avatar);
    gtk_widget_class_bind_template_child(widget_class, UchatTextMessage, message);
    gtk_widget_class_bind_template_child(widget_class, UchatTextMessage, time);
}

void
uchat_text_message_set_message(UchatTextMessage* self, const gchar* message) {
    gtk_label_set_label(GTK_LABEL(self->message), message);
}

const gchar *
uchat_text_message_get_message(UchatTextMessage* self) {
    return gtk_label_get_label(GTK_LABEL(self->message));
}

void
uchat_text_message_set_time(UchatTextMessage* self, const gchar* time) {
    gtk_label_set_label(GTK_LABEL(self->time), time);
}

const gchar *
uchat_message_box_get_time(UchatTextMessage* self) {
    return gtk_label_get_label(GTK_LABEL(self->time));
}

static void
uchat_text_message_init(UchatTextMessage *self) {
    gtk_widget_init_template(GTK_WIDGET(self));
}

UchatTextMessage *
uchat_text_message_new(t_message* message, bool own) {
    UchatTextMessage* obj = g_object_new(UCHAT_TYPE_TEXT_MESSAGE, NULL);

    gtk_widget_set_visible(obj->avatar, !own);
    gtk_widget_set_halign(GTK_WIDGET(obj), own ? GTK_ALIGN_END : GTK_ALIGN_START);
    uchat_text_message_set_message(obj, message->content);
    uchat_text_message_set_time(obj, strndup(&(message->time[11]), 5));

    return obj;
}

