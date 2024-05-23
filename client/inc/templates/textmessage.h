#ifndef UCHAT_TEXT_MESSAGE_H
#define UCHAT_TEXT_MESSAGE_H

#include "models/message.h"
#include "uchat.h"

G_BEGIN_DECLS

#define UCHAT_TYPE_TEXT_MESSAGE uchat_text_message_get_type()

G_DECLARE_FINAL_TYPE(UchatTextMessage, uchat_text_message, UCHAT, TEXT_MESSAGE, GtkWidget)

UchatTextMessage *
uchat_text_message_new(t_message* message, bool own);

void
uchat_text_message_set_author(UchatTextMessage* self, const gchar* author);

const gchar *
uchat_text_message_get_author(UchatTextMessage* self);

void
uchat_text_message_set_message(UchatTextMessage* self, const gchar* message);

const gchar *
uchat_text_message_get_message(UchatTextMessage* self);

void
uchat_text_message_set_time(UchatTextMessage* self, const gchar* time);

const gchar *
uchat_text_message_get_time(UchatTextMessage* self);

G_END_DECLS

#endif

