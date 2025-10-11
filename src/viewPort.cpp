#include <gtk-2.0/gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <glib-2.0/glib.h>
#include <cairo/cairo.h>
#include <cstdlib>
#include "dungeonCell.h"
#include "viewPort.h"
#include "data/tile_null.h"
#include "data/tile_floor_base.h"
#include "data/tile_wall_bottom.h"
#include "data/tile_wall_top.h"
#include "data/tile_wall_right.h"
#include "data/tile_wall_top_right.h"
#include "data/tile_wall_left.h"
#include "data/tile_wall_top_left.h"
#include "data/tile_wall_right_left.h"
#include "data/tile_wall_top_right_left.h"
#include "data/tile_cell_selected.h"

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
extern const guint8 tile_floor_base[];
extern const guint8 tile_wall_top[];
extern const guint8 tile_wall_top_right[];
extern const guint8 tile_wall_right[];
extern const guint8 tile_wall_bottom[];
extern const guint8 tile_wall_left[];
extern const guint8 tile_wall_top_left[];
extern const guint8 tile_wall_right_left[];
extern const guint8 tile_wall_top_right_left[];
extern const guint8 tile_cell_selected[];

GdkPixbuf *tiles[TILE_COUNT] = {NULL};
GtkWidget *viewPort = NULL;
Point viewPosition = {0}; // The dungeonCell position of the viewPort origin.
Point selectedCell = {0}; // The current player-selected dungeonCell in the viewPort.

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

static TILE GetWallTile(gint positionX, gint positionY);
static GdkPixbuf* GetTileForCellSelected(gint positionX, gint positionY);
static GdkPixbuf* GetTileForTerrain(gint positionX, gint positionY);
static GdkPixbuf* GetTileForCell(gint positionX, gint positionY);
static GdkPixbuf* GetPixbufFromTile(TILE tile);

// ------------------------------------------------------------------------------------------------
//
void InitViewPort(void)
{
    LoadImagesToPixbufs();
    viewPort = gtk_drawing_area_new();
    gtk_widget_set_size_request(viewPort, VIEWPORT_WIDTH * TILE_SIZE, VIEWPORT_HEIGHT * TILE_SIZE);
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
// Sets the dungeonCell position of the viewPort origin such that the given position is centered.
void CenterViewPositionOn(gint positionX, gint positionY)
{
    SetViewPosition(positionX - VIEWPORT_WIDTH / 2, positionY - VIEWPORT_HEIGHT / 2);
}

// ------------------------------------------------------------------------------------------------
// Gets the dungeonCell position of the currently selected cell in the viewPort.
Point* GetSelectedCell(void)
{
    return &selectedCell;
}

// ------------------------------------------------------------------------------------------------
// Sets the currently selected dungeonCell in the viewPort to the given position.
void SetSelectedCell(gint positionX, gint positionY)
{
    selectedCell.x = positionX;
    selectedCell.y = positionY;
}

// ------------------------------------------------------------------------------------------------
// Returns the array of image data required for gdk_pixbuf_new_from_inline.
const guint8* GetTileData(enum TILE tile)
{
    switch (tile)
    {
    case TILE_NULL:
        return tile_null;
    case TILE_WALL_TOP:
        return tile_wall_top;
    case TILE_WALL_RIGHT:
        return tile_wall_right;
    case TILE_WALL_TOP_RIGHT:
        return tile_wall_top_right;
    case TILE_WALL_BOTTOM:
        return tile_wall_bottom;
    case TILE_WALL_LEFT:
        return tile_wall_left;
    case TILE_WALL_TOP_LEFT:
        return tile_wall_top_left;
    case TILE_WALL_RIGHT_LEFT:
        return tile_wall_right_left;
    case TILE_WALL_TOP_RIGHT_LEFT:
        return tile_wall_top_right_left;
    case TILE_FLOOR_BASE:
        return tile_floor_base;
    case TILE_CELL_SELECTED:
        return tile_cell_selected;
    case TILE_COUNT:
        return NULL;
    }
    return NULL;
}

// ------------------------------------------------------------------------------------------------
// Returns the tile image for a TERRAIN_WALL cell based on the surrounding cells.
static TILE GetWallTile(gint positionX, gint positionY)
{
    guint tileMask = 0;
    TERRAIN cellUp = GetCellTerrain(positionX, positionY - 1);
    TERRAIN cellRight = GetCellTerrain(positionX + 1, positionY);
    TERRAIN cellLeft = GetCellTerrain(positionX - 1, positionY);
    TERRAIN cellDown = GetCellTerrain(positionX, positionY + 1);
    TERRAIN cell45 = GetCellTerrain(positionX + 1, positionY - 1);
    TERRAIN cell135 = GetCellTerrain(positionX + 1, positionY + 1);
    TERRAIN cell225 = GetCellTerrain(positionX - 1, positionY - 1);
    TERRAIN cell315 = GetCellTerrain(positionX - 1, positionY + 1);

    if (cellUp == TERRAIN_FLOOR)
        tileMask +=1;
    if (cellRight == TERRAIN_FLOOR)
        tileMask +=2;
    if (cellDown == TERRAIN_FLOOR)
        tileMask +=4;
    if (cellLeft == TERRAIN_FLOOR)
        tileMask +=8;

    switch ((TILE_MASK)tileMask)
    {
    case MASK_TOP:
        if (cell135 == TERRAIN_FLOOR && cell315 == TERRAIN_FLOOR)
            return TILE_WALL_TOP_RIGHT_LEFT;
        else if (cell135 == TERRAIN_FLOOR)
            return TILE_WALL_TOP_RIGHT;
        else if (cell315 == TERRAIN_FLOOR)
            return TILE_WALL_TOP_LEFT;
        else
            return TILE_WALL_TOP;
    case MASK_RIGHT:
        if (cell225 == TERRAIN_FLOOR && cell315 == TERRAIN_FLOOR)
            return TILE_WALL_RIGHT_LEFT;
        else
            return TILE_WALL_RIGHT;
    case MASK_BOTTOM:
    case MASK_TOP_BOTTOM:
    case MASK_TOP_RIGHT_BOTTOM:
    case MASK_TOP_BOTTOM_LEFT:
    case MASK_TOP_RIGHT_BOTTOM_LEFT:
    case MASK_RIGHT_BOTTOM_LEFT:
    case MASK_BOTTOM_LEFT:
    case MASK_RIGHT_BOTTOM:
        return TILE_WALL_BOTTOM;
    case MASK_LEFT:
        if (cell45 == TERRAIN_FLOOR && cell135 == TERRAIN_FLOOR)
            return TILE_WALL_RIGHT_LEFT;
        else
            return TILE_WALL_LEFT;
    case MASK_RIGHT_LEFT:
        return TILE_WALL_RIGHT_LEFT;
    case MASK_TOP_RIGHT:
        return TILE_WALL_TOP_RIGHT;
    case MASK_TOP_LEFT:
        return TILE_WALL_TOP_LEFT;
    case MASK_TOP_RIGHT_LEFT:
        return TILE_WALL_TOP_RIGHT_LEFT;
    default:
        if (cell135 == TERRAIN_FLOOR && cell315 == TERRAIN_FLOOR)
            return TILE_WALL_RIGHT_LEFT;
        else if (cell45 == TERRAIN_FLOOR && cell135 == TERRAIN_FLOOR)
            return TILE_WALL_RIGHT;
        else if (cell225 == TERRAIN_FLOOR && cell315 == TERRAIN_FLOOR)
            return TILE_WALL_LEFT;
        else
        return TILE_NULL;
    }
}

// ------------------------------------------------------------------------------------------------
// Returns the GdkPixbuf from the tiles array for the dungeonCell selected indicator.
static GdkPixbuf* GetTileForCellSelected(gint positionX, gint positionY)
{
    return GetPixbufFromTile(TILE_CELL_SELECTED);
}

// ------------------------------------------------------------------------------------------------
// Returns the GdkPixbuf from the tiles array for the given cell based on its terrain.
static GdkPixbuf* GetTileForTerrain(gint positionX, gint positionY)
{
    TERRAIN terrain = GetCellTerrain(positionX, positionY);
    TILE tile;

    switch (terrain)
    {
    case TERRAIN_FLOOR:
        tile = TILE_FLOOR_BASE;
        break;
    case TERRAIN_WALL:
        tile = GetWallTile(positionX, positionY);
        break;
    default:
        tile = TILE_NULL;
    }

    return GetPixbufFromTile(tile);
}

// ------------------------------------------------------------------------------------------------
// Returns the GdkPixbuf from the tiles array based on the dungeonCell's contents.
static GdkPixbuf* GetTileForCell(gint positionX, gint positionY)
{
    Point *selectedCell = GetSelectedCell();

    // If cell is selected, return tile for selected cell marker.
    if (selectedCell->x == positionX && selectedCell->y == positionY)
    {
        return GetTileForCellSelected(positionX, positionY);
    }
    else // Cell is not selected, return terrain tile.
    {
        return GetTileForTerrain(positionX, positionY);
    }

    return GetPixbufFromTile(TILE_NULL);
}

// ------------------------------------------------------------------------------------------------
// Returns a GdkPixbuf based on the given idex to the tiles array.
static GdkPixbuf* GetPixbufFromTile(TILE tile)
{
    return tiles[tile];
}

// ------------------------------------------------------------------------------------------------
// Read image data into the GdkPixbufs tiles array.
void LoadImagesToPixbufs(void)
{
    GError * error = NULL;
    for (guint i = 0; i < TILE_COUNT; i++)
    {
        tiles[i] = gdk_pixbuf_new_from_inline(-1, GetTileData((enum TILE)i), FALSE, &error);
        tiles[i] = gdk_pixbuf_scale_simple(tiles[i], TILE_SIZE, TILE_SIZE, GDK_INTERP_NEAREST);
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

// ------------------------------------------------------------------------------------------------
// Callback function to update the tiles shown on the viewPort.
gboolean UpdateViewPort(GtkWidget *widget, cairo_t *context, gpointer userData)
{
    // Get the GdkWindow from the widget
    GdkWindow *window = gtk_widget_get_window(widget);

    if (window)
    {
        // Create a Cairo context from the GdkWindow
        cairo_t *context = gdk_cairo_create(window);
        Point *viewPosition = GetViewPosition();

        for (gint y = 0; y < VIEWPORT_HEIGHT; y++)
        {
            for (gint x = 0; x < VIEWPORT_WIDTH; x++)
            {
                // The pixel position within the viewPort.
                gint viewPortX = TILE_SIZE * x;
                gint viewPortY = TILE_SIZE * y;

                // The cell position within the dungeon.
                gint cellX = viewPosition->x + x;
                gint cellY = viewPosition->y + y;

                gdk_cairo_set_source_pixbuf(context, GetTileForCell(cellX, cellY), viewPortX, viewPortY);
                cairo_paint(context);
            }
        }

        // Clean up the Cairo context
        cairo_destroy(context);
    }
    return FALSE;
}
