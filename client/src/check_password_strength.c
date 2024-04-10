#include "password.h"

static bool str_has_digit(const char* str) {
    for(int i = 0; i < strlen(str); i++) {
        if(isdigit(str[i])) {
            return true;
        }
    }

    return false;
}

static bool str_has_lower(const char* str) {
    for(int i = 0; i < strlen(str); i++) {
        if(islower(str[i])) {
            return true;
        }
    }

    return false;
}

static bool str_has_upper(const char* str) {
    for(int i = 0; i < strlen(str); i++) {
        if(isupper(str[i])) {
            return true;
        }
    }

    return false;
}

static bool str_has_special(const char* str) {
    for(int i = 0; i < strlen(str); i++) {
        if(ispunct(str[i])) {
            return true;
        }
    }

    return false;
}

bool* check_password_strength(const char* password, bool* strength) {
    bool* res = (bool *)malloc(PWD_NUM * sizeof(bool));

    res[PWD_LENGTH] = strlen(password) > 7;
    res[PWD_DIGIT] = str_has_digit(password);
    res[PWD_LOWER] = str_has_lower(password);
    res[PWD_UPPER] = str_has_upper(password);
    res[PWD_SPECIAL] = str_has_special(password);
    
    *strength = res[PWD_LENGTH]
             && res[PWD_DIGIT]
             && res[PWD_LOWER]
             && res[PWD_UPPER]
             && res[PWD_SPECIAL];
    
    return res;
}

