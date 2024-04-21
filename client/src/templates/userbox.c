#include "templates/userbox.h"

struct _UchatUserBox {
    GtkWidget parent_instance;

    t_user* user;
    GtkWidget* username;
};

G_DEFINE_TYPE(UchatUserBox, uchat_user_box, GTK_TYPE_WIDGET)

static void
uchat_user_box_class_init(UchatUserBoxClass *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

    gtk_widget_class_set_template_from_resource(widget_class, "/ua/ucode-connect/uchat/ui/userbox.ui");
    gtk_widget_class_set_layout_manager_type(widget_class, GTK_TYPE_BOX_LAYOUT);

    gtk_widget_class_bind_template_child(widget_class, UchatUserBox, username);
}

void
uchat_user_box_set_user(UchatUserBox* self, t_user* user) {
    self->user = user;
}

t_user *
uchat_user_box_get_user(UchatUserBox* self) {
    return self->user;
}

void
uchat_user_box_set_username(UchatUserBox* self, const gchar* username) {
    gtk_label_set_label(GTK_LABEL(self->username), username);
}

const gchar *
uchat_user_box_get_username(UchatUserBox* self) {
    return gtk_label_get_label(GTK_LABEL(self->username));
}

static void
uchat_user_box_init(UchatUserBox *self) {
    gtk_widget_init_template(GTK_WIDGET(self));
}

UchatUserBox *
uchat_user_box_new(t_user* user) {
    UchatUserBox* obj = g_object_new(UCHAT_TYPE_USER_BOX, NULL);

    uchat_user_box_set_user(obj, user);
    uchat_user_box_set_username(obj, user->username);

    return obj;
}

