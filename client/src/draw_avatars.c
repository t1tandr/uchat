#include "uchat.h"

char* ava_path = NULL;

static void draw_from_path(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data) {
    unsigned long size;
    unsigned char *from_bytes = g_base64_decode(ava_path,&size);
    int result = mkdir("storage/chat12", 0777);
    const char* path = "storage/chat12/12";
    bytes_to_file(from_bytes,size,path);

    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(path, NULL);
    
    int img_width = gdk_pixbuf_get_width(pixbuf);
    int img_height = gdk_pixbuf_get_height(pixbuf);

    cairo_arc(cr, width / 2.0, height / 2.0, MIN(width, height) / 2.0, 0, 2 * G_PI);
    cairo_clip(cr);
    cairo_new_path(cr);
    double scale_factor = 0,offset_x = 0,offset_y = 0;
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

static void on_open_response (GtkDialog *dialog, int response, gpointer user_data) {
    t_user* user = (t_user *)malloc(sizeof(t_user));
    if (response == GTK_RESPONSE_ACCEPT)
        {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);

        GtkFileFilter *filter = gtk_file_filter_new();
        gtk_file_filter_set_name(filter, "Image");
        gtk_file_filter_add_mime_type(filter, "image/png");
        gtk_file_filter_add_mime_type(filter, "image/jpeg");

        gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

        g_autoptr(GFile) file = gtk_file_chooser_get_file (chooser);
        const gchar *path = g_file_get_path(file);
        long size;
        unsigned char* p = file_to_bytes(path,&size);
        ava_path = g_base64_encode(p,size);
        g_free((gpointer)path);
        GtkBox* box_in_setting = GTK_BOX(gtk_builder_get_object(uchat->builder, "box_for_image_in_settings"));

        //Надо везде добавить где отрисовывать аватар
        GtkWidget *area1 = GTK_WIDGET(gtk_builder_get_object(uchat->builder, "avatar-in-settings"));
        gtk_drawing_area_set_content_width (GTK_DRAWING_AREA (area1),126);
        gtk_drawing_area_set_content_height (GTK_DRAWING_AREA (area1), 126);
        gtk_drawing_area_set_draw_func (GTK_DRAWING_AREA (area1),
                                        draw_from_path,
                                        user_data, NULL);
        gtk_widget_queue_draw(area1);

        GtkWidget *area = GTK_WIDGET(gtk_builder_get_object(uchat->builder, "avatar-in-overlay"));
        gtk_drawing_area_set_content_width (GTK_DRAWING_AREA (area),90);
        gtk_drawing_area_set_content_height (GTK_DRAWING_AREA (area), 90);
        gtk_drawing_area_set_draw_func (GTK_DRAWING_AREA (area),
                                        draw_from_path,
                                        user_data, NULL);
        gtk_widget_queue_draw(area);
        }
    gtk_window_destroy(GTK_WINDOW (dialog));
}

void open_file_chooser(GtkButton* self, gpointer user_data) {
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(uchat->builder, "main-window"));
    
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;

    dialog = gtk_file_chooser_dialog_new ("Open File",
                                      window,
                                      action,
                                      ("_Cancel"),
                                      GTK_RESPONSE_CANCEL,
                                      ("_Open"),
                                      GTK_RESPONSE_ACCEPT,
                                      NULL);

    gtk_window_set_transient_for(GTK_WINDOW(dialog),GTK_WINDOW(window));
    g_signal_connect(dialog, "response", G_CALLBACK(on_open_response), user_data);
    gtk_widget_show(GTK_WIDGET(dialog));
}