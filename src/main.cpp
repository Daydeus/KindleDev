#include <gtk-2.0/gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <glib-2.0/glib.h>
#include <cairo/cairo.h>
#include <cstdlib>
#include <time.h>
#include "dungeonCell.h"
#include "viewPort.h"
#include "dungeonGeneration.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------


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

extern TERRAIN dungeonCells[DUNGEON_WIDTH * DUNGEON_HEIGHT];

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

void on_button_up(GtkWidget *widget);
void on_button_down(GtkWidget *widget);
void on_button_left(GtkWidget *widget);
void on_button_right(GtkWidget *widget);
void on_button_generate(GtkWidget *widget);
void SetBackgroundColor(GtkWidget *widget, enum Color colorName);

// ------------------------------------------------------------------------------------------------
// The main application loop.
int main(int argc, char *argv[])
{
    // Randomize dungeonCells.
    srand(time(NULL));

    gtk_init(&argc, &argv);
    g_print("ViewPosition is: (%d, %d).\n", viewPosition.x, viewPosition.y);

    // Initialize non-global Gtk widgets.
    GtkWindow *applicationMain = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));
    InitViewPort();
    GtkVBox *vboxMain = GTK_VBOX(gtk_vbox_new(TRUE, 0));
    GtkAlignment *viewPortAlign = GTK_ALIGNMENT(gtk_alignment_new(0.5, 0, 0 ,0));
    GtkHBox  *hboxControls = GTK_HBOX(gtk_hbox_new(TRUE, 0));
    GtkButton *buttonUp = GTK_BUTTON(gtk_button_new_with_label("Move Up"));
    GtkButton *buttonDown = GTK_BUTTON(gtk_button_new_with_label("Move Down"));
    GtkButton *buttonLeft = GTK_BUTTON(gtk_button_new_with_label("Move Left"));
    GtkButton *buttonRight = GTK_BUTTON(gtk_button_new_with_label("Move Right"));
    GtkButton *buttonGenerate = GTK_BUTTON(gtk_button_new_with_label("Generate\nMap"));
    GtkButton *buttonQuit = GTK_BUTTON(gtk_button_new_with_label("Quit"));

    // Add widgets to containers.
    gtk_container_add(GTK_CONTAINER(applicationMain), GTK_WIDGET(vboxMain));
    gtk_box_pack_start(GTK_BOX(vboxMain), GTK_WIDGET(viewPortAlign), FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(viewPortAlign), GTK_WIDGET(viewPort));
    gtk_box_pack_start(GTK_BOX(vboxMain), GTK_WIDGET(hboxControls), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hboxControls), GTK_WIDGET(buttonUp), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hboxControls), GTK_WIDGET(buttonDown), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hboxControls), GTK_WIDGET(buttonLeft), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hboxControls), GTK_WIDGET(buttonRight), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hboxControls), GTK_WIDGET(buttonGenerate), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hboxControls), GTK_WIDGET(buttonQuit), FALSE, FALSE, 0);

    // Exit the application when the main window is closed or the quit button pressed.
    g_signal_connect(applicationMain, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(viewPort), "expose_event", G_CALLBACK(UpdateViewPort), NULL);
    g_signal_connect(buttonUp, "button_press_event", G_CALLBACK(on_button_up), NULL);
    g_signal_connect(buttonDown, "button_press_event", G_CALLBACK(on_button_down), NULL);
    g_signal_connect(buttonLeft, "button_press_event", G_CALLBACK(on_button_left), NULL);
    g_signal_connect(buttonRight, "button_press_event", G_CALLBACK(on_button_right), NULL);
    g_signal_connect(buttonGenerate, "button_press_event", G_CALLBACK(on_button_generate), NULL);
    g_signal_connect(buttonQuit, "button_press_event", G_CALLBACK(gtk_main_quit), NULL);

    // Set the intial options before applicationMain is made visible.
    gtk_window_set_title(GTK_WINDOW(applicationMain), "L:A_N:application_ID:kindle-gtk_PC:T");
    SetBackgroundColor(GTK_WIDGET(applicationMain), COLOR_WHITE);
    gtk_window_maximize(GTK_WINDOW(applicationMain));
    SetBackgroundColor(GTK_WIDGET(viewPort), COLOR_WHITE);

    gtk_widget_show_all(GTK_WIDGET(applicationMain));

    gtk_main();

    // Exiting program.
    FreePixbufs();

    return 0;
}

// ------------------------------------------------------------------------------------------------
// Moves the viewPort origin position up one space and updates the viewPieces.
void on_button_up(GtkWidget *widget)
{
    MoveViewPosition(DIR_UP, 1);
    gtk_widget_queue_draw(viewPort);
    g_print("ViewPosition is: (%d, %d).\n", viewPosition.x, viewPosition.y);
}

// ------------------------------------------------------------------------------------------------
// Moves the viewPort origin position down one space and updates the viewPieces.
void on_button_down(GtkWidget *widget)
{
    MoveViewPosition(DIR_DOWN, 1);
    gtk_widget_queue_draw(viewPort);
    g_print("ViewPosition is: (%d, %d).\n", viewPosition.x, viewPosition.y);
}

// ------------------------------------------------------------------------------------------------
// Moves the viewPort origin position left one space and updates the viewPieces.
void on_button_left(GtkWidget *widget)
{
    MoveViewPosition(DIR_LEFT, 1);
    gtk_widget_queue_draw(viewPort);
    g_print("ViewPosition is: (%d, %d).\n", viewPosition.x, viewPosition.y);
}

// ------------------------------------------------------------------------------------------------
// Moves the viewPort origin position right one space and updates the viewPieces.
void on_button_right(GtkWidget *widget)
{
    MoveViewPosition(DIR_RIGHT, 1);
    gtk_widget_queue_draw(viewPort);
    g_print("ViewPosition is: (%d, %d).\n", viewPosition.x, viewPosition.y);
}

// ------------------------------------------------------------------------------------------------
// Generates a new dungeon to explore.
void on_button_generate(GtkWidget *widget)
{
    GenerateDungeon();
    gtk_widget_queue_draw(viewPort);
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
