#ifndef UCHAT_MESSAGE_BOX_H
#define UCHAT_MESSAGE_BOX_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define UCHAT_TYPE_MESSAGE_BOX uchat_message_box_get_type()

G_DECLARE_FINAL_TYPE(UchatMessageBox, uchat_message_box, UCHAT, MESSAGE_BOX, GtkWidget)

UchatMessageBox *
uchat_message_box_new(const char* message, struct tm* time);

void
uchat_message_box_set_author(UchatMessageBox* self, const gchar* author);

const gchar *
uchat_message_box_get_author(UchatMessageBox* self);

void
uchat_message_box_set_message(UchatMessageBox* self, const gchar* message);

const gchar *
uchat_message_box_get_message(UchatMessageBox* self);

void
uchat_message_box_set_time(UchatMessageBox* self, struct tm *time);

const char *
uchat_message_box_get_time(UchatMessageBox* self);

void
uchat_message_box_set_seen(UchatMessageBox* self, gboolean seen);

gboolean
uchat_message_box_get_seen(UchatMessageBox* self);

G_END_DECLS

#endif