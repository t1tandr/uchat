#ifndef UCHAT_CHAT_BOX_H
#define UCHAT_CHAT_BOX_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define UCHAT_TYPE_CHAT_BOX uchat_chat_box_get_type()

G_DECLARE_FINAL_TYPE(UchatChatBox, uchat_chat_box, UCHAT, CHAT_BOX, GtkWidget)

UchatChatBox *
uchat_chat_box_new(const char* name);

void
uchat_chat_box_set_name(UchatChatBox* self, const gchar* name);

const gchar *
uchat_chat_box_get_name(UchatChatBox* self);

void
uchat_chat_box_set_message(UchatChatBox* self, const gchar* message);

const gchar *
uchat_chat_box_get_message(UchatChatBox* self);

void
uchat_chat_box_set_time(UchatChatBox* self, const gchar* time);

const gchar *
uchat_chat_box_get_time(UchatChatBox* self);

void
uchat_chat_box_set_seen(UchatChatBox* self, gboolean seen);

gboolean
uchat_chat_box_get_seen(UchatChatBox* self);

G_END_DECLS

#endif
