#ifndef UCHAT_AVATAR_BOX_H
#define UCHAT_AVATAR_BOX_H

#include "uchat.h"

G_BEGIN_DECLS

#define UCHAT_TYPE_AVATAR_BOX uchat_avatar_box_get_type()

G_DECLARE_FINAL_TYPE(UchatAvatarBox, uchat_avatar_box, UCHAT, AVATAR_BOX, GtkWidget)

UchatAvatarBox *
uchat_avatar_box_new(const gchar* path, int size);

void
uchat_avatar_box_set_file(UchatAvatarBox* self, const gchar* path);

const gchar *
uchat_avatar_box_get_file(UchatAvatarBox* self);

G_END_DECLS

#endif

