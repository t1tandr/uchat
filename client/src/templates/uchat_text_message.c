#include "uchat_text_message.h"

struct _UchatTextMessage {
  GtkWidget parent_instance;

  GtkWidget* author;
  GtkWidget* message;
  GtkWidget* time;
};

G_DEFINE_TYPE(UchatTextMessage, uchat_text_message, GTK_TYPE_WIDGET)

static void
uchat_text_message_class_init(UchatTextMessageClass *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

    gtk_widget_class_set_template_from_resource(widget_class, "/ua/ucode-connect/uchat/message.ui");
    gtk_widget_class_set_layout_manager_type (widget_class, GTK_TYPE_BOX_LAYOUT);
    gtk_widget_class_bind_template_child(widget_class, UchatTextMessage, author);
}

void 
uchat_text_message_set_author(UchatTextMessage* self, GtkWidget* author) {
    self->author = author;
}

GtkWidget * 
uchat_text_message_get_author(UchatTextMessage* self) {
    return self->author;
}

static void
uchat_text_message_init(UchatTextMessage *self) {
    gtk_widget_init_template(GTK_WIDGET(self));
}

UchatTextMessage *
uchat_text_message_new(const char* author) {
    UchatTextMessage* obj = g_object_new(UCHAT_TYPE_TEXT_MESSAGE, NULL);

    gtk_label_set_label(GTK_LABEL(obj->author), author);

    return obj;
}