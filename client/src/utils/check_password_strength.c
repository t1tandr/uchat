#include "uchat.h"

static bool str_has_digit(const char* str) {
    for (int i = 0; i < strlen(str); i++) {
        if (isdigit(str[i])) {
            return true;
        }
    }

    return false;
}

static bool str_has_lower(const char* str) {
    for (int i = 0; i < strlen(str); i++) {
        if (islower(str[i])) {
            return true;
        }
    }

    return false;
}

static bool str_has_upper(const char* str) {
    for (int i = 0; i < strlen(str); i++) {
        if (isupper(str[i])) {
            return true;
        }
    }

    return false;
}

static bool str_has_special(const char* str) {
    for (int i = 0; i < strlen(str); i++) {
        if (ispunct(str[i])) {
            return true;
        }
    }

    return false;
}

static void check_password_requirement(bool req, GtkBuilder* builder, int id) {
    GtkWidget* label = GTK_WIDGET(gtk_builder_get_object(builder, req_fields[id][BUILDER_ID]));
    
    if (!req) {
        gtk_widget_add_css_class(label, "error-label");
        gtk_label_set_label(GTK_LABEL(label), req_fields[id][WRONG_LABEL]);
    }
    else {
        if (widget_has_css_class(label, "error-label")) {
            gtk_widget_remove_css_class(label, "error-label");
        }
        gtk_label_set_label(GTK_LABEL(label), req_fields[id][CORRECT_LABEL]);
    }
}

bool check_password_strength(const char* password, GtkBuilder* builder) {
    bool* reqs = (bool *)malloc(PWD_NUM * sizeof(bool));

    reqs[PWD_LENGTH] = strlen(password) > 7;
    reqs[PWD_DIGIT] = str_has_digit(password);
    reqs[PWD_LOWER] = str_has_lower(password);
    reqs[PWD_UPPER] = str_has_upper(password);
    reqs[PWD_SPECIAL] = str_has_special(password);
    
    for (int i = 0; i < PWD_NUM; i++) {
        check_password_requirement(reqs[i], builder, i);
    }

    return reqs[PWD_LENGTH] 
        && reqs[PWD_DIGIT] 
        && reqs[PWD_LOWER] 
        && reqs[PWD_UPPER] 
        && reqs[PWD_SPECIAL];
}

