#ifndef _UCHAT_MESSAGE_BOX
#define _UCHAT_MESSAGE_BOX

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define UCHAT_TYPE_MESSAGE_BOX uchat_message_box_get_type()

G_DECLARE_FINAL_TYPE(UchatMessageBox, uchat_message_box, UCHAT, MESSAGE_BOX, GtkWidget)

UchatMessageBox *
uchat_message_box_new(const char* name);

void
uchat_message_box_set_author(UchatMessageBox* self, const gchar* author);

const gchar *
uchat_message_box_get_author(UchatMessageBox* self);

void
uchat_message_box_set_message(UchatMessageBox* self, const gchar* message);

const gchar *
uchat_message_box_get_message(UchatMessageBox* self);

void
uchat_message_box_set_time(UchatMessageBox* self, const gchar* time);

const gchar *
uchat_message_box_get_time(UchatMessageBox* self);

void
uchat_message_box_set_seen(UchatMessageBox* self, gboolean seen);

gboolean
uchat_message_box_get_seen(UchatMessageBox* self);

G_END_DECLS

#endif