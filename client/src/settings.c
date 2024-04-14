#include "uchat.h"

char *path = NULL;

static void draw_from_path(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data) {
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

void return_from_settings_button_click_cb(GtkButton* self, gpointer user_data) {
    GtkBuilder* builder = GTK_BUILDER(user_data);
    GtkBox* homepage = GTK_BOX(gtk_builder_get_object(builder, "home-page"));
    GtkBox* right_side_menu = GTK_BOX(gtk_builder_get_object(builder, "right-side-menu"));
    GtkBox* settings = GTK_BOX(gtk_builder_get_object(builder, "settings"));

    gtk_box_remove(homepage, GTK_WIDGET(settings));
    gtk_box_append(homepage, GTK_WIDGET(right_side_menu));
}

void on_response(GtkDialog *dialog, gint response_id, gpointer user_data) {
    if (response_id == GTK_RESPONSE_OK) {
        GdkRGBA color;
        gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(dialog),&color);
        GtkBuilder* builder = GTK_BUILDER(user_data);
        GtkWidget* box = GTK_WIDGET(gtk_builder_get_object(builder, "BoxForImage"));
        GtkWidget* box_1 = GTK_WIDGET(gtk_builder_get_object(builder, "box_for_image_in_settings"));
        char *css = g_strdup_printf("box { background-color: %s;border-radius: 50%%; }",gdk_rgba_to_string(&color));

        GtkCssProvider *provider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(provider, css, -1);
        gtk_style_context_add_provider(gtk_widget_get_style_context(box),GTK_STYLE_PROVIDER(provider),GTK_STYLE_PROVIDER_PRIORITY_USER);
        gtk_style_context_add_provider(gtk_widget_get_style_context(box_1),GTK_STYLE_PROVIDER(provider),GTK_STYLE_PROVIDER_PRIORITY_USER);
        g_free(css);
        gtk_widget_hide(GTK_WIDGET(dialog));
    } 
    else if (response_id == GTK_RESPONSE_CANCEL) {
        gtk_widget_hide(GTK_WIDGET(dialog));
    }
}

void open_color_chooser(GtkButton* self, gpointer user_data) {
    GtkBuilder* builder = GTK_BUILDER(user_data);
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "main-window"));
    
    GtkColorChooserDialog *dialog = GTK_COLOR_CHOOSER_DIALOG(gtk_color_chooser_dialog_new("Select Color", GTK_WINDOW(window)));
    gtk_window_set_transient_for(GTK_WINDOW(dialog),GTK_WINDOW(window));
    g_signal_connect(dialog, "response", G_CALLBACK(on_response), user_data);
    gtk_widget_show(GTK_WIDGET(dialog));
}

static void on_open_response (GtkDialog *dialog, int response, gpointer user_data) {
    GtkBuilder* builder = GTK_BUILDER(user_data);
    if (response == GTK_RESPONSE_ACCEPT)
        {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);

        GtkFileFilter *filter = gtk_file_filter_new();
        gtk_file_filter_set_name(filter, "Image");
        gtk_file_filter_add_mime_type(filter, "image/png");
        gtk_file_filter_add_mime_type(filter, "image/jpeg");

        gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

        g_autoptr(GFile) file = gtk_file_chooser_get_file (chooser);

        path = g_file_get_path(file);


        GtkBox* box = GTK_BOX(gtk_builder_get_object(builder, "BoxForImage"));
        GtkBox* box_in_setting = GTK_BOX(gtk_builder_get_object(builder, "box_for_image_in_settings"));

        GtkWidget* child = gtk_widget_get_first_child(GTK_WIDGET(box));
        gtk_box_remove(box,GTK_WIDGET(child));

        GtkWidget* child_box_in_setting = gtk_widget_get_first_child(GTK_WIDGET(box_in_setting));
        gtk_box_remove(box_in_setting,GTK_WIDGET(child_box_in_setting));

        GtkWidget *area = gtk_drawing_area_new ();
        gtk_drawing_area_set_content_width (GTK_DRAWING_AREA (area),326);
        gtk_drawing_area_set_content_height (GTK_DRAWING_AREA (area), 326);
        gtk_drawing_area_set_draw_func (GTK_DRAWING_AREA (area),
                                        draw_from_path,
                                        NULL, NULL);
        gtk_widget_set_overflow(area,TRUE);                            
        gtk_widget_set_halign(area,TRUE);
        gtk_widget_set_valign(area,TRUE);
        gtk_widget_set_margin_start(area,5);
        gtk_widget_set_margin_end(area,5);
        gtk_widget_set_margin_top(area,5);
        gtk_widget_set_margin_bottom(area,5);
        gtk_widget_queue_draw(area);
        gtk_box_append(GTK_BOX(box),area);
        gtk_widget_show(area);

        GtkWidget *area1 = gtk_drawing_area_new ();
        gtk_drawing_area_set_content_width (GTK_DRAWING_AREA (area1),126);
        gtk_drawing_area_set_content_height (GTK_DRAWING_AREA (area1), 126);
        gtk_drawing_area_set_draw_func (GTK_DRAWING_AREA (area1),
                                        draw_from_path,
                                        NULL, NULL);
        gtk_widget_set_overflow(area1,TRUE);                            
        gtk_widget_set_halign(area1,TRUE);
        gtk_widget_set_valign(area1,TRUE);
        gtk_widget_set_margin_start(area1,5);
        gtk_widget_set_margin_end(area1,5);
        gtk_widget_set_margin_top(area1,5);
        gtk_widget_set_margin_bottom(area1,5);
        gtk_widget_queue_draw(area1);
        gtk_box_append(GTK_BOX(box_in_setting),area1);
        gtk_widget_show(area1);
        }

    gtk_window_destroy(GTK_WINDOW (dialog));
}

void open_file_chooser(GtkButton* self, gpointer user_data) {
    GtkBuilder* builder = GTK_BUILDER(user_data);
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "main-window"));
    
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
