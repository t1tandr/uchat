#include "templates/messagebox.h"

static gchar* text_view_get_full_text(GtkTextView* view) {
    GtkTextBuffer* buffer = gtk_text_view_get_buffer(view);
    GtkTextIter start, end;
    gchar* text = NULL;

    gtk_text_buffer_get_bounds(buffer, &start, &end);

    text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

    gtk_text_buffer_delete(buffer, &start, &end);

    return text;
}

struct _UchatMessageBox {
  GtkWidget parent_instance;

  t_chat* chat;
  GtkWidget* textview;
  GtkWidget* container;
  GtkWidget* name;
  GtkWidget* nmembers;
};

G_DEFINE_TYPE(UchatMessageBox, uchat_message_box, GTK_TYPE_WIDGET)

static void
uchat_message_box_class_init(UchatMessageBoxClass *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

    gtk_widget_class_set_template_from_resource(widget_class, "/ua/ucode-connect/uchat/ui/messagebox.ui");
    gtk_widget_class_set_layout_manager_type(widget_class, GTK_TYPE_BOX_LAYOUT);

    gtk_widget_class_bind_template_child(widget_class, UchatMessageBox, container);
    gtk_widget_class_bind_template_child(widget_class, UchatMessageBox, name);
    gtk_widget_class_bind_template_child(widget_class, UchatMessageBox, nmembers);
    gtk_widget_class_bind_template_child(widget_class, UchatMessageBox, textview);
}

gchar *
uchat_message_box_get_text(UchatMessageBox* self) {
    return text_view_get_full_text(self->textview);
}

t_chat *
uchat_message_box_get_chat(UchatMessageBox* self) {
    return self->chat;
}

void
uchat_message_box_add_message(UchatMessageBox* self, t_message* message, bool own) {
    UchatTextMessage* msg = uchat_text_message_new(message, own);
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
    gtk_label_set_label(GTK_LABEL(obj->nmembers), mx_itoa(mx_list_size(chat->members)));

    return obj;
}

