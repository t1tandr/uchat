#ifndef _UCHAT_CONTACT_BOX
#define _UCHAT_CONTACT_BOX

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define UCHAT_TYPE_CONTACT_BOX uchat_contact_box_get_type()

G_DECLARE_FINAL_TYPE(UchatContactBox, uchat_contact_box, UCHAT, CONTACT_BOX, GtkWidget)

UchatContactBox *
uchat_contact_box_new(const char* author);

void 
uchat_contact_box_set_author(UchatContactBox* self, const char* author);

const char * 
uchat_contact_box_get_author(UchatContactBox* self);

G_END_DECLS

#endif
