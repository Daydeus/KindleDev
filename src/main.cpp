#include <cstdlib>
#include <gtk-2.0/gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <glib-2.0/glib.h>
#include <time.h>
#include "global.h"
#include "actor.h"
#include "dungeonCell.h"
#include "dungeonGeneration.h"
#include "menu.h"
#include "pathfinding.h"
#include "tile.h"
#include "viewPort.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// The main application loop.
int main(int argc, char *argv[])
{
    // Randomize dungeonCells.
    srand(time(NULL));

    gtk_init(&argc, &argv);

    // Initialize non-global Gtk widgets.
    GtkWindow *applicationMain = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));
    GtkAlignment *alignMain = GTK_ALIGNMENT(gtk_alignment_new(0.5, 0, 0 ,0));
    GtkTable *tableMain = GTK_TABLE(gtk_table_new(5, 1, FALSE));
    gtk_container_add(GTK_CONTAINER(applicationMain), GTK_WIDGET(alignMain));
    gtk_container_add(GTK_CONTAINER(alignMain), GTK_WIDGET(tableMain));

    // Set up the viewPort and menu.
    LoadBorderTiles();
    InitViewPort();
    InitMenu();
    LoadColorFillTiles();
    gtk_table_attach(tableMain, GTK_WIDGET(viewPort), 0, 1, 0, 1, GTK_SHRINK, GTK_SHRINK, 0, 0);
    gtk_table_attach(tableMain, GTK_WIDGET(menu), 0, 1, 1, 2, GTK_SHRINK, GTK_SHRINK, 0, 0);
    gtk_table_set_row_spacing (GTK_TABLE(tableMain), 0, TILE_SIZE_16);

    // Connect widget signals to callbacks.
    g_signal_connect(applicationMain, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Set the intial options before applicationMain is made visible.
    gtk_window_set_title(GTK_WINDOW(applicationMain), "L:A_N:application_ID:kindle-gtk_PC:T");
    SetWidgetBgColor(GTK_WIDGET(applicationMain), COLOR_WHITE);
    gtk_window_maximize(GTK_WINDOW(applicationMain));

    // Init game.
    GenerateDungeon();
    CenterViewPortOn(&actors[PLAYER_ACTOR_INDEX].position);

    gtk_widget_show_all(GTK_WIDGET(applicationMain));

    gtk_main();

    // Exiting program.
    FreeAllTiles();

    return 0;
}
