#include "uchat.h"

void password_entry_changed_cb(GtkEntry* self, gpointer user_data) {
    GtkBuilder* builder = GTK_BUILDER(user_data);
    const char* password = gtk_editable_get_text(GTK_EDITABLE(self));
    bool strength = false;
    bool* reqs = check_password_strength(password, &strength);
    GtkRevealer* revealer = GTK_REVEALER(gtk_builder_get_object(builder, "password-reqs-revealer"));

    gtk_revealer_set_reveal_child(revealer, !strength || gtk_revealer_get_child_revealed(revealer));

    for(int i = 0; i < PWD_NUM; i++) {
        check_password_requirement(reqs[i], builder, i);
    }
}

