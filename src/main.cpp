#include <gtk-2.0/gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <glib-2.0/glib.h>
#include <cstdlib>
#include <time.h>
#include "dungeonCell.h"
#include "viewPort.h"

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
// Global Variables
// ------------------------------------------------------------------------------------------------

static GtkWindow *applicationMain;
static GtkButton *buttonRandomize;
static GtkButton *buttonQuit;
extern TERRAIN dungeonCells[DUNGEON_WIDTH * DUNGEON_HEIGHT];

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

void SetBackgroundColor(GtkWidget *widget, enum Color colorName);

// ------------------------------------------------------------------------------------------------
// The main application loop.
int main(int argc, char *argv[])
{
    // Randomize dungeonCells.
    srand(time(NULL));

    gtk_init(&argc, &argv);

    // Initialize global Gtk widgets.
    applicationMain = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));
    buttonRandomize = GTK_BUTTON(gtk_button_new_with_label("Randomize"));
    buttonQuit = GTK_BUTTON(gtk_button_new_with_label("Quit"));
    InitViewPort();
    UpdateViewPieces();

    // Initialize non-global Gtk widgets.
    GtkVBox *vbox = GTK_VBOX(gtk_vbox_new(TRUE, 0));
    GtkAlignment *viewPortAlign = GTK_ALIGNMENT(gtk_alignment_new(0.5, 0, 0 , 0));

    // Add widgets to containers.
    gtk_container_add(GTK_CONTAINER(applicationMain), GTK_WIDGET(vbox));
    gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(viewPortAlign), FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(viewPortAlign), GTK_WIDGET(viewPort));
    gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(buttonRandomize), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(buttonQuit), FALSE, FALSE, 0);

    // Exit the application when the main window is closed or the quit button pressed.
    g_signal_connect(applicationMain, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(buttonRandomize, "button_press_event", G_CALLBACK(RandomizeDungeon), NULL);
    g_signal_connect(buttonQuit, "button_press_event", G_CALLBACK(gtk_main_quit), NULL);

    // Set the intial options before applicationMain is made visible.
    gtk_window_set_title(GTK_WINDOW(applicationMain), "L:A_N:application_ID:kindle-gtk_PC:T");
    SetBackgroundColor(GTK_WIDGET(applicationMain), COLOR_WHITE);
    gtk_window_maximize(GTK_WINDOW(applicationMain));

    gtk_widget_show_all(GTK_WIDGET(applicationMain));

    gtk_main();

    FreePixbufs();

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
