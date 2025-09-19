#include <gtk-2.0/gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <glib-2.0/glib.h>
#include <cstdlib>
#include "dungeonCell.h"
#include "viewPort.h"
#include "tile_null.h"
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

extern const guint8 tile_null[];
extern const guint8 tile_zero[];
extern const guint8 tile_one[];
GtkTable *viewPort = NULL;
GtkImage *viewPieces[VIEWPORT_WIDTH * VIEWPORT_HEIGHT] = {NULL};
GdkPixbuf *tiles[TILE_COUNT] = {NULL};
Point viewPosition = {0};

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Initializes the GtkTable, GdkPixbufs, and GtkImages required for the viewPort to function.
void InitViewPort(void)
{
    // Initialize the viewPort GtkTable.
    viewPort = GTK_TABLE(gtk_table_new(VIEWPORT_HEIGHT, VIEWPORT_WIDTH, TRUE));

    // Load image data to Pixbufs.
   LoadImagesToPixbufs();

    // Initialize the viewPieces GtkImage and attach to viewPort.
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
void UpdateViewPieces(void)
{
    Point *originPos = GetViewPosition();

    for (guint y = 0; y < VIEWPORT_HEIGHT; y++)
    {
        for (guint x = 0; x < VIEWPORT_WIDTH; x++)
        {
            guint index = (y * VIEWPORT_WIDTH) + x;

            if (IsOutsideDungeon(originPos->x + x, originPos->y + y))
            {
                gtk_image_set_from_pixbuf(viewPieces[index], tiles[TILE_NULL]);
            }
            else
            {
                guint tileIndex = (guint)GetCellTerrain(originPos->x + x, originPos->y + y);
                gtk_image_set_from_pixbuf(viewPieces[index], tiles[tileIndex]);
            }
        }
    }
}

// ------------------------------------------------------------------------------------------------
// Gets the dungeonCell position of the viewPort origin.
Point* GetViewPosition(void)
{
    return &viewPosition;

}

// ------------------------------------------------------------------------------------------------
// Sets the dungeonCell position of the viewPort origin to the given values.
void SetViewPosition(gint positionX, gint positionY)
{
    viewPosition.x = positionX;
    viewPosition.y = positionY;

}

// ------------------------------------------------------------------------------------------------
// Moves the dungeonCell position of the viewPort origin based on the given direction and distance.
void MoveViewPosition(DIRECTION direction, guint distance)
{
    Point *position = GetViewPosition();

    switch (direction)
    {
    case DIR_UP:
        position->y -= distance;
        break;
    case DIR_DOWN:
        position->y += distance;
        break;
    case DIR_LEFT:
        position->x -= distance;
        break;
    case DIR_RIGHT:
        position->x += distance;
        break;
    default:
        break;
    }

    SetViewPosition(position->x, position->y);
}

// ------------------------------------------------------------------------------------------------
// Returns the array of image data required for gdk_pixbuf_new_from_inline.
const guint8* GetTileData(enum TILE tile)
{
    switch (tile)
    {
    case TILE_NULL:
        return tile_null;
    case TILE_ZERO:
        return tile_zero;
    case TILE_ONE:
        return tile_one;
    case TILE_COUNT:
        return NULL;
    }
    return NULL;
}

// ------------------------------------------------------------------------------------------------
// Read image data into the GdkPixbufs tiles array.
void LoadImagesToPixbufs(void)
{
    GError * error = NULL;
    for (guint i = 0; i < TILE_COUNT; i++)
    {
        tiles[i] = gdk_pixbuf_new_from_inline(-1, GetTileData((enum TILE)i), FALSE, &error);

        #ifdef KINDLE_BUILD
        tiles[i] = gdk_pixbuf_scale_simple(tiles[i], SCALE_SIZE, SCALE_SIZE, GDK_INTERP_NEAREST);
        #endif
    }
}

// ------------------------------------------------------------------------------------------------
// Free the GdkPixbufs for the tiles array.
void FreePixbufs(void)
{
    // Free memory used by GdkPixbufs.
    for (guint i = 0; i < TILE_COUNT; i++)
    {
        g_object_unref(tiles[i]);
    }
}
