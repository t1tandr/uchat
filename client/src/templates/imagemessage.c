#include "templates/imagemessage.h"

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
    gtk_picture_set_filename(GTK_PICTURE(self->image), path);
}

GFile*
uchat_image_message_get_image(UchatImageMessage* self) {
    return gtk_picture_get_file(GTK_PICTURE(self->image));
}

void
uchat_image_message_set_time(UchatImageMessage* self, const gchar* time) {
    gtk_label_set_label(GTK_LABEL(self->time), strndup(&(time[11]), 5));
}

const gchar *
uchat_image_message_get_time(UchatImageMessage* self) {
    return gtk_label_get_label(GTK_LABEL(self->time));
}

void
uchat_image_message_set_avatar(UchatImageMessage* self, const gchar* path) {
    uchat_avatar_box_set_file(UCHAT_AVATAR_BOX(self->avatar), path);
}

const gchar *
uchat_image_message_get_avatar(UchatImageMessage* self) {
    return uchat_avatar_box_get_file(UCHAT_AVATAR_BOX(self->avatar));
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

    uchat_image_message_set_image(obj, message->content);
    uchat_image_message_set_avatar(obj, message->avatar);
    uchat_image_message_set_time(obj, message->time);


    return obj;
}
