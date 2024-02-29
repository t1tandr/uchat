#ifndef UCHAT_H
#define UCHAT_H

#include <gtk/gtk.h>
#include <regex.h>
#include <ctype.h>

typedef struct sign_in_form {
    GtkWindow *window;
    GtkEntry  *username;
    GtkEntry  *password;
}   sign_in_form;

#endif

