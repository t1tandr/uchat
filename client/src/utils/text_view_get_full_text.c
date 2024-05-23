#include "uchat.h"

gchar* text_view_get_full_text(GtkTextView* view) {
    GtkTextBuffer* buffer = gtk_text_view_get_buffer(view);
    GtkTextIter start, end;
    gchar* text = NULL;

    gtk_text_buffer_get_bounds(buffer, &start, &end);

    text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

    gtk_text_buffer_delete(buffer, &start, &end);

    return text;
}

