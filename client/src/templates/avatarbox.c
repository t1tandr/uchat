#include "templates/avatarbox.h"

enum {
    PROP_SIZE = 1,
    N_PROPS
};

static GParamSpec *properties[N_PROPS] = { NULL, };

static void 
draw_from_path(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data) {
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(user_data, NULL);

    int img_width = gdk_pixbuf_get_width(pixbuf);
    int img_height = gdk_pixbuf_get_height(pixbuf);

    cairo_arc(cr, width / 2.0, height / 2.0, MIN(width, height) / 2.0, 0, 2 * G_PI);
    cairo_clip(cr);
    cairo_new_path(cr);
    double scale_factor = 0, offset_x = 0, offset_y = 0;

    if(img_width == img_height){
        scale_factor = MIN((double) width / img_width, (double) height / img_height);
        offset_x = (width - img_width * scale_factor) / 2.0;
        offset_y = (height - img_height * scale_factor) / 2.0;
    }
    else{
        scale_factor = MIN(((double) width / img_width * 1.5), ((double) height / img_height)*1.5);
        offset_x = (width - img_width * scale_factor);
        offset_y = (height - img_height * scale_factor);
    }

    cairo_translate(cr, offset_x, offset_y);
    cairo_scale(cr, scale_factor, scale_factor);
    gdk_cairo_set_source_pixbuf(cr, pixbuf, 0, 0);
    cairo_paint(cr);

    g_object_unref(pixbuf);
}

struct _UchatAvatarBox {
    GtkWidget parent_instance;

    int size;
    GFile* file;
    GtkWidget* area;
};

G_DEFINE_TYPE(UchatAvatarBox, uchat_avatar_box, GTK_TYPE_WIDGET)

static void
uchat_avatar_box_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec) {
    UchatAvatarBox *self = UCHAT_AVATAR_BOX(object);

    switch (property_id) {
        case PROP_SIZE:
            self->size = g_value_get_int(value);
            gtk_drawing_area_set_content_width(GTK_DRAWING_AREA(self->area), self->size);
            gtk_drawing_area_set_content_height(GTK_DRAWING_AREA(self->area), self->size);
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
            break;
    }
}

static void
uchat_avatar_box_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec) {
    UchatAvatarBox *self = UCHAT_AVATAR_BOX(object);

    switch (property_id) {
        case PROP_SIZE:
            g_value_set_int(value, self->size);
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
            break;
    }
}

static void
uchat_avatar_box_class_init(UchatAvatarBoxClass *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

    gtk_widget_class_set_template_from_resource(widget_class, "/ua/ucode-connect/uchat/ui/avatarbox.ui");
    gtk_widget_class_set_layout_manager_type(widget_class, GTK_TYPE_BOX_LAYOUT);

    G_OBJECT_CLASS(widget_class)->set_property = uchat_avatar_box_set_property;
    G_OBJECT_CLASS(widget_class)->get_property = uchat_avatar_box_get_property;

    properties[PROP_SIZE] =
    g_param_spec_int("size",
                    "Size",
                    "Size of drawing area content",
                    0,
                    300,
                    0,
                    G_PARAM_READWRITE);

    g_object_class_install_properties(G_OBJECT_CLASS(widget_class), N_PROPS, properties);

    gtk_widget_class_bind_template_child(widget_class, UchatAvatarBox, area);
}

void 
uchat_avatar_box_set_file(UchatAvatarBox* self, GFile* file) {
    self->file = file;
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(self->area), draw_from_path, g_file_get_path(file), NULL);
    gtk_widget_queue_draw(self->area);
}

GFile * 
uchat_avatar_box_get_file(UchatAvatarBox* self) {
    return self->file;
}

static void
uchat_avatar_box_init(UchatAvatarBox *self) {
    gtk_widget_init_template(GTK_WIDGET(self));
}

UchatAvatarBox *
uchat_avatar_box_new(GFile* file, int size) {
    UchatAvatarBox* obj = g_object_new(UCHAT_TYPE_AVATAR_BOX, NULL);

    uchat_avatar_box_set_file(obj, file);

    gtk_widget_set_size_request(GTK_WIDGET(obj), size, size);
    gtk_drawing_area_set_content_width(GTK_DRAWING_AREA(obj->area), size);
    gtk_drawing_area_set_content_height(GTK_DRAWING_AREA(obj->area), size);

    return obj;
}

