#ifndef UCHAT_USER_BOX_H
#define UCHAT_USER_BOX_H

#include <gtk/gtk.h>
#include "models/user.h"

G_BEGIN_DECLS

#define UCHAT_TYPE_USER_BOX uchat_user_box_get_type()

G_DECLARE_FINAL_TYPE(UchatUserBox, uchat_user_box, UCHAT, USER_BOX, GtkWidget)

UchatUserBox *
uchat_user_box_new(t_user* user);

void
uchat_user_box_set_user(UchatUserBox* self, t_user* user);

t_user *
uchat_user_box_get_user(UchatUserBox* self);

void
uchat_user_box_set_username(UchatUserBox* self, const gchar* username);

const gchar *
uchat_user_box_get_username(UchatUserBox* self);

G_END_DECLS

#endif

