#ifndef _UCHAT_TEXT_MESSAGE
#define _UCHAT_TEXT_MESSAGE

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define UCHAT_TYPE_TEXT_MESSAGE uchat_text_message_get_type()

G_DECLARE_FINAL_TYPE(UchatTextMessage, uchat_text_message, UCHAT, TEXT_MESSAGE, GtkWidget)

UchatTextMessage *
uchat_text_message_new(const char* author);

void 
uchat_text_message_set_author(UchatTextMessage* self, GtkWidget* author);

GtkWidget * 
uchat_text_message_get_author(UchatTextMessage* self);

G_END_DECLS

#endif
