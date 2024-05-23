#ifndef UCHAT_MESSAGE_BOX_H
#define UCHAT_MESSAGE_BOX_H

#include <gtk/gtk.h>
#include "uchat.h"

G_BEGIN_DECLS

#define UCHAT_TYPE_MESSAGE_BOX uchat_message_box_get_type()

G_DECLARE_FINAL_TYPE(UchatMessageBox, uchat_message_box, UCHAT, MESSAGE_BOX, GtkWidget)

UchatMessageBox *
uchat_message_box_new(t_chat* chat);

t_chat *
uchat_message_box_get_chat(UchatMessageBox* self);

void
uchat_message_box_add_message(UchatMessageBox* self, t_message* message, bool own);

gchar *
uchat_message_box_get_text(UchatMessageBox* self);

void
uchat_message_box_add_image(UchatMessageBox* self, t_message* message, bool own);

G_END_DECLS

#endif

