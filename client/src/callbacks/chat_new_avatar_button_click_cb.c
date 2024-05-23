#include "uchat.h"

static void on_save_response_cb(GtkDialog *self, int response, gpointer user_data) {
    if (response == GTK_RESPONSE_ACCEPT) {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(self);
        UchatAvatarBox* avatar = UCHAT_AVATAR_BOX(gtk_builder_get_object(uchat->builder, "chat-new-avatar"));

        g_autoptr(GFile) file = gtk_file_chooser_get_file(chooser);

        uchat_avatar_box_set_file(avatar, file);
    }

    gtk_window_destroy(GTK_WINDOW(self));
}

void chat_new_avatar_button_click_cb(GtkButton* self, gpointer user_data) {
    GtkWidget* dialog = NULL;
    GtkFileFilter* filter = NULL;

    dialog = gtk_file_chooser_dialog_new("Choose image",
                                        GTK_WINDOW(gtk_builder_get_object(uchat->builder, "chat-new-dialog")),
                                        GTK_FILE_CHOOSER_ACTION_OPEN,
                                        "Cancel",
                                        GTK_RESPONSE_CANCEL,
                                        "Save",
                                        GTK_RESPONSE_ACCEPT,
                                        NULL);

    filter = gtk_file_filter_new();
    gtk_file_filter_add_pixbuf_formats(filter);
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    g_signal_connect(dialog, "response", G_CALLBACK(on_save_response_cb), user_data);

    gtk_window_present(GTK_WINDOW(dialog));
}

