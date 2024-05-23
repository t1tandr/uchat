#ifndef UTILS_H
#define UTILS_H

#include <gtk/gtk.h>
#include "models/message.h"

bool widget_has_css_class(GtkWidget* self, const char* class);
void window_switch_child(GtkBuilder* builder, char* prev_id, char* next_id);
gchar* text_view_get_full_text(GtkTextView* view);

void add_css_stylesheet(const char* path);
void add_icon_theme(const char* path);

void login_form_clear(GtkBuilder* builder);
void register_form_clear(GtkBuilder* builder);

GtkBuilder* builder_create(const char* files[]);

void create_session_file(const char* filename, cJSON* obj);

void add_message_to_message_box(t_message* message);

#endif

