#include "uchat_contact_box.h"

struct _UchatContactBox {
  GtkWidget parent_instance;

  GtkWidget* author;
  GtkWidget* message;
  GtkWidget* time;
};

G_DEFINE_TYPE(UchatContactBox, uchat_contact_box, GTK_TYPE_WIDGET)

static void
uchat_contact_box_class_init(UchatContactBoxClass *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

    gtk_widget_class_set_template_from_resource(widget_class, "/ua/ucode-connect/uchat/ui/contact-box.ui");
    gtk_widget_class_set_layout_manager_type (widget_class, GTK_TYPE_BOX_LAYOUT);
    gtk_widget_class_bind_template_child(widget_class, UchatContactBox, author);
}

void 
uchat_contact_box_set_author(UchatContactBox* self, const char* author) {
    gtk_label_set_label(GTK_LABEL(self->author), author);
}

const char * 
uchat_contact_box_get_author(UchatContactBox* self) {
    return gtk_label_get_label(GTK_LABEL(self->author));
}

static void
uchat_contact_box_init(UchatContactBox *self) {
    gtk_widget_init_template(GTK_WIDGET(self));
}

UchatContactBox *
uchat_contact_box_new(const char* author) {
    UchatContactBox* obj = g_object_new(UCHAT_TYPE_CONTACT_BOX, NULL);

    gtk_label_set_label(GTK_LABEL(obj->author), author);

    return obj;
}