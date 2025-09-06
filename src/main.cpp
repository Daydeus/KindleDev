#include <gtk-2.0/gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <glib-2.0/glib.h>
#include <cstdlib>

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------

//#define KINDLE_HF_BUILD 1

// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------

// Colors available for the application to use.
enum Color
{
    COLOR_BLACK,
    COLOR_GREY_DARK,
    COLOR_GREY_LIGHT,
    COLOR_WHITE
};

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

void SetBackgroundColor(GtkWidget *widget, enum Color colorName);

// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

static GtkWindow *applicationMain;
static GtkButton *buttonQuit;

// ------------------------------------------------------------------------------------------------
// The main application loop.
int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    // Initialize static Gtk widgets.
    applicationMain = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));
    buttonQuit = GTK_BUTTON(gtk_button_new_with_label("Quit"));

    gtk_container_add(GTK_CONTAINER(applicationMain), GTK_WIDGET(buttonQuit));

    // Exit the application when the main window is closed or the quit button pressed.
    g_signal_connect(applicationMain, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(buttonQuit, "button_press_event", G_CALLBACK(gtk_main_quit), NULL);

    // Set the intial options before applicationMain is made visible.
    gtk_window_set_title(GTK_WINDOW(applicationMain), "L:A_N:application_ID:kindle-gtk_PC:T");
    SetBackgroundColor(GTK_WIDGET(applicationMain), COLOR_WHITE);
    gtk_window_maximize(GTK_WINDOW(applicationMain));

    gtk_widget_show_all(GTK_WIDGET(applicationMain));

    gtk_main();

    return 0;
}

// ------------------------------------------------------------------------------------------------
// Sets the widget's background color.
void SetBackgroundColor(GtkWidget *widget, enum Color colorName)
{
    const gchar* string;
    GdkColor color;

    switch (colorName)
    {
    case COLOR_BLACK:
        string = "#000000";
    break;
    case COLOR_GREY_DARK:
        string = "#444444";
    break;
    case COLOR_GREY_LIGHT:
        string = "#bbbbbb";
    break;
    case COLOR_WHITE:
        string = "#ffffff";
    break;
    }

    if (gdk_color_parse(string, &color))
    {
        gtk_widget_modify_bg(widget, GTK_STATE_NORMAL, &color);
    }
}
