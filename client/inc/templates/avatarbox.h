#ifndef UCHAT_AVATAR_BOX_H
#define UCHAT_AVATAR_BOX_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define UCHAT_TYPE_AVATAR_BOX uchat_avatar_box_get_type()

G_DECLARE_FINAL_TYPE(UchatAvatarBox, uchat_avatar_box, UCHAT, AVATAR_BOX, GtkWidget)

UchatAvatarBox *
uchat_avatar_box_new(GFile* file, int size);

void
uchat_avatar_box_set_file(UchatAvatarBox* self, GFile* file);

GFile *
uchat_avatar_box_get_file(UchatAvatarBox* self);

G_END_DECLS

#endif

