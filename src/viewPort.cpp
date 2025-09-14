#include <gtk-2.0/gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <glib-2.0/glib.h>
#include <cstdlib>
#include "dungeonCell.h"
#include "viewPort.h"
#include "tile_zero.h"
#include "tile_one.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

extern const guint8 tile_zero[];
extern const guint8 tile_one[];

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Initializes the viewPieces and adds them to table cells in the viewPort.
extern void InitViewPieces(GtkTable *viewPort, GtkImage *viewPieces[])
{
    for (guint y = 0; y < VIEWPORT_HEIGHT; y++)
    {
        for (guint x = 0; x < VIEWPORT_WIDTH; x++)
        {
            guint index = (y * VIEWPORT_WIDTH) + x;

            viewPieces[index] = GTK_IMAGE(gtk_image_new());
            gtk_table_attach(viewPort, GTK_WIDGET(viewPieces[index]), x, x+1, y, y+1,
                             GTK_SHRINK, GTK_SHRINK, 0, 0);
        }
    }
}

// ------------------------------------------------------------------------------------------------
// Changes the pixbuf the GtkImage viewPiece points to based on the contents of the dungeonCells.
extern void UpdateViewPieces(GtkImage *viewPieces[], GdkPixbuf **tiles)
{
    for (guint y = 0; y < VIEWPORT_HEIGHT; y++)
    {
        for (guint x = 0; x < VIEWPORT_WIDTH; x++)
        {
            guint index = (y * VIEWPORT_WIDTH) + x;
            guint tileIndex = (guint)GetCellTerrain(x, y);

            gtk_image_set_from_pixbuf(viewPieces[index], tiles[tileIndex]);
        }
    }
}

// ------------------------------------------------------------------------------------------------
// Returns the array of image data required for gdk_pixbuf_new_from_inline.
extern const guint8* GetTileData(enum TILE tile)
{
    switch (tile)
    {
    case TILE_ZERO:
        return tile_zero;
    case TILE_ONE:
        return tile_one;
    }
}
