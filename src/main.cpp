#include <gtk-2.0/gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <glib-2.0/glib.h>
#include <cstdlib>
#include "viewPort.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------

//#define KINDLE_HF_BUILD 1
#define SCALE_SIZE 48

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
static GtkButton *buttonQuit;

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

void SetBackgroundColor(GtkWidget *widget, enum Color colorName);

// ------------------------------------------------------------------------------------------------
// The main application loop.
int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    // Initialize global Gtk widgets.
    applicationMain = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));
    buttonQuit = GTK_BUTTON(gtk_button_new_with_label("Quit"));

    // Initialize non-global Gtk widgets.
    GtkTable *viewPort = GTK_TABLE(gtk_table_new(VIEWPORT_HEIGHT, VIEWPORT_WIDTH, TRUE));
    GtkImage *viewPieces[VIEWPORT_WIDTH * VIEWPORT_HEIGHT];
    InitViewPieces(viewPort, viewPieces);
    GtkVBox *vbox = GTK_VBOX(gtk_vbox_new(TRUE, 0));

    // Initialize GdkPixbufs.
    GError * error = NULL;
    GdkPixbuf *tiles[TILE_COUNT];
    for (guint i = 0; i < TILE_COUNT; i++)
    {
        tiles[i] = gdk_pixbuf_new_from_inline(-1, GetTileData((enum TILE)i), FALSE, &error);
        tiles[i] = gdk_pixbuf_scale_simple(tiles[i], SCALE_SIZE, SCALE_SIZE, GDK_INTERP_NEAREST);
    }

    // Set the viewPieces to show pixbufs.
    for (guint y = 0; y < VIEWPORT_HEIGHT; y++)
    {
        for (guint x = 0; x < VIEWPORT_WIDTH; x++)
        {
            guint index = (y * VIEWPORT_WIDTH) + x;

            if (x % 2 == 0)
                gtk_image_set_from_pixbuf(viewPieces[index], tiles[TILE_ZERO]);
            else
                gtk_image_set_from_pixbuf(viewPieces[index], tiles[TILE_ONE]);
        }
    }

    // Add widgets to containers.
    gtk_container_add(GTK_CONTAINER(applicationMain), GTK_WIDGET(vbox));
    gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(viewPort), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(buttonQuit), FALSE, FALSE, 0);

    // Exit the application when the main window is closed or the quit button pressed.
    g_signal_connect(applicationMain, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(buttonQuit, "button_press_event", G_CALLBACK(gtk_main_quit), NULL);

    // Set the intial options before applicationMain is made visible.
    gtk_window_set_title(GTK_WINDOW(applicationMain), "L:A_N:application_ID:kindle-gtk_PC:T");
    SetBackgroundColor(GTK_WIDGET(applicationMain), COLOR_WHITE);
    gtk_window_maximize(GTK_WINDOW(applicationMain));

    gtk_widget_show_all(GTK_WIDGET(applicationMain));

    gtk_main();

    // Free memory used by GdkPixbufs.
    for (guint i = 0; i < TILE_COUNT; i++)
    {
        g_object_unref(tiles[i]);
    }

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
