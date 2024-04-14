#include "uchat.h"

static void draw_function(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer data) {
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("resources/img/background.png", NULL);

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

void register_button_click_cb(GtkButton* self, gpointer user_data) {
    GtkBuilder* builder = GTK_BUILDER(user_data);
    GtkRevealer* revealer = GTK_REVEALER(gtk_builder_get_object(builder, "username-is-taken-revealer"));

    gtk_revealer_set_reveal_child(revealer, TRUE);

    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "main-window"));
    GtkWidget* home_page = GTK_WIDGET(gtk_builder_get_object(builder, "home-page"));
    GtkWidget* login_page = GTK_WIDGET(gtk_builder_get_object(builder, "login-page"));

    gtk_widget_unparent(login_page);
    gtk_window_set_child(window, home_page);
    GtkBox* box = GTK_BOX(gtk_builder_get_object(builder, "BoxForImage"));
    GtkBox* box_in_setting = GTK_BOX(gtk_builder_get_object(builder, "box_for_image_in_settings"));

    GtkWidget *area = gtk_drawing_area_new ();
    gtk_drawing_area_set_content_width (GTK_DRAWING_AREA (area),326);
    gtk_drawing_area_set_content_height (GTK_DRAWING_AREA (area), 326);
    gtk_drawing_area_set_draw_func (GTK_DRAWING_AREA (area),
                                    draw_function,
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
                                    draw_function,
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


