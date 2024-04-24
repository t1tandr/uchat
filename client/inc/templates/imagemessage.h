#ifndef UCHAT_IMAGE_MESSAGE_H
#define UCHAT_IMAGE_MESSAGE_H

#include <gtk/gtk.h>
#include "uchat.h"

G_BEGIN_DECLS

#define UCHAT_TYPE_IMAGE_MESSAGE uchat_image_message_get_type()

G_DECLARE_FINAL_TYPE(UchatImageMessage, uchat_image_message, UCHAT, IMAGE_MESSAGE, GtkWidget)

UchatImageMessage*
uchat_image_message_new(t_message* message, bool own);

void
uchat_image_message_set_author(UchatImageMessage* self, const gchar* author);

const gchar *
uchat_image_message_get_author(UchatImageMessage* self);

void
uchat_image_message_set_image(UchatImageMessage* self, const gchar* path);

GFile*
uchat_image_message_get_image(UchatImageMessage* self);

const gchar *
uchat_message_box_get_timee(UchatImageMessage* self);

void
uchat_image_message_set_time(UchatImageMessage* self, const gchar* time);

G_END_DECLS

#endif
