#ifndef UTILS_H
#define UTILS_H

#include <gtk/gtk.h>

void handle_error(const char* message);

bool widget_has_css_class(GtkWidget* self, const char* class);
void window_switch_child(GtkBuilder* builder, char* prev_id, char* next_id);

void add_css_stylesheet(const char* path);
void add_icon_theme(const char* path);

void login_form_clear(GtkBuilder* builder);
void register_form_clear(GtkBuilder* builder);

GtkBuilder* builder_create(const char* files[]);

void create_session_file(const char* filename, cJSON* obj);

#endif

