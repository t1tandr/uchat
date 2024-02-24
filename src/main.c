#include <gtk/gtk.h>
#include <regex.h>
#include <ctype.h>

typedef struct sign_in_form {
    GtkWindow *window;
    GtkEntry  *username;
    GtkEntry  *password;
}   sign_in_form;

/*

static bool check_password_strength(const char *password) {
    int nupper = 0;
    int nlower = 0;
    int ndigit = 0;
    int nspecial = 0;
    
    for(int i = 0; password[i]; i++) {
        char c = password[i];

        if(isupper(c)) nupper++;
        if(islower(c)) nlower++;
        if(isdigit(c)) ndigit++;
        if(ispunct(c)) nspecial++;
   }
   return nupper && nlower && ndigit && nspecial;
}

*/

static void sign_in_button_click(GtkWidget *button, gpointer data) {
    GtkWidget *dialog, *username_label, *password_label, *content_area;
    sign_in_form *form = (sign_in_form *)data;
    char username_buf[1024] = { 0 };
    char password_buf[1024] = { 0 };

    gtk_button_set_label(GTK_BUTTON(button), "Sign in");

    dialog = gtk_dialog_new_with_buttons("Authentification", form->window, GTK_DIALOG_DESTROY_WITH_PARENT, "OK", GTK_RESPONSE_NONE, NULL);
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
    gtk_window_set_default_size(GTK_WINDOW(dialog), 300, 200);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_widget_set_valign(content_area, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(content_area, GTK_ALIGN_CENTER);

    sprintf(username_buf, "USERNAME: %s", gtk_entry_buffer_get_text(gtk_entry_get_buffer(form->username)));
    sprintf(password_buf, "PASSWORD: %s", gtk_entry_buffer_get_text(gtk_entry_get_buffer(form->password)));

    username_label = gtk_label_new(username_buf);
    gtk_widget_set_valign(username_label, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(username_label, GTK_ALIGN_START);

    gtk_box_append(GTK_BOX(content_area), username_label);

    password_label = gtk_label_new(password_buf);
    gtk_widget_set_valign(password_label, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(password_label, GTK_ALIGN_START);

    gtk_box_append(GTK_BOX(content_area), password_label);

    gtk_window_set_child(GTK_WINDOW(dialog), content_area);
    gtk_window_present(GTK_WINDOW(dialog));
}

static void app_activate(GtkApplication *app) {
    GtkWidget *window = NULL;
    GtkWidget *vbox = NULL;
    GtkWidget *username_entry = NULL;
    GtkWidget *password_entry = NULL;
    GtkWidget *username_label = NULL;
    GtkWidget *password_label = NULL;
    GtkWidget *sign_in_button = NULL;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "UChat");
    gtk_window_maximize(GTK_WINDOW(window));

    username_label = gtk_label_new("Username");
    gtk_widget_set_halign(username_label, GTK_ALIGN_START);

    username_entry = gtk_entry_new();

    password_label = gtk_label_new("Password");
    gtk_widget_set_halign(password_label, GTK_ALIGN_START);

    password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_valign(vbox, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(vbox, GTK_ALIGN_CENTER);

    sign_in_button = gtk_button_new_with_label("Sign in");

    gtk_box_append(GTK_BOX(vbox), username_label);
    gtk_box_append(GTK_BOX(vbox), username_entry);
    gtk_box_append(GTK_BOX(vbox), password_label);
    gtk_box_append(GTK_BOX(vbox), password_entry);
    gtk_box_append(GTK_BOX(vbox), sign_in_button);

    gtk_window_set_child(GTK_WINDOW(window), vbox);

    sign_in_form *sf = (sign_in_form *)malloc(sizeof(struct sign_in_form));
    sf->window = GTK_WINDOW(window);
    sf->username = GTK_ENTRY(username_entry);
    sf->password = GTK_ENTRY(password_entry);

    g_signal_connect(G_OBJECT(sign_in_button), "clicked", G_CALLBACK(sign_in_button_click), sf);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char *argv[]) {
    GtkApplication *app = gtk_application_new("ua.ucode-connect.uchat", G_APPLICATION_FLAGS_NONE);

    g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);

    return g_application_run(G_APPLICATION(app), argc, argv);
}

