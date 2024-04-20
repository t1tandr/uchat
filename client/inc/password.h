#ifndef PASSWORD_H
#define PASSWORD_H

#include <gtk/gtk.h>
#include <ctype.h>

enum {
    PWD_LENGTH,
    PWD_DIGIT,
    PWD_LOWER,
    PWD_UPPER,
    PWD_SPECIAL,
    PWD_NUM
};

enum {
    BUILDER_ID,
    CORRECT_LABEL,
    WRONG_LABEL,
};

static const char* req_fields[PWD_NUM][3] = {
    { "password-length",  "✔ Contains at least 8 characters", "x Contains at least 8 characters" },
    { "password-digit",   "✔ Contains a digit",               "x Contains a digit" },
    { "password-lower",   "✔ Contains a lowercase letter",    "x Contains a lowercase letter" },
    { "password-upper",   "✔ Contains an uppercase letter",   "x Contains an uppercase letter" },
    { "password-special", "✔ Contains a special symbol",      "x Contains a special symbol" }
};

bool check_password_strength(const char* password, GtkBuilder* builder);

#endif

