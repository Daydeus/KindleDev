#include <gtk-2.0/gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <glib-2.0/glib.h>
#include <cstdlib>
#include "dungeonCell.h"
#include "viewPort.h"
#include "tile_null.h"
#include "tile_floor_base.h"
#include "tile_wall_bottom.h"
#include "tile_wall_top.h"
#include "tile_wall_right.h"
#include "tile_wall_top_right.h"
#include "tile_wall_left.h"
#include "tile_wall_top_left.h"
#include "tile_wall_right_left.h"
#include "tile_wall_top_right_left.h"

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
GtkTable *viewPort = NULL;
GtkImage *viewPieces[VIEWPORT_WIDTH * VIEWPORT_HEIGHT] = {NULL};
GdkPixbuf *tiles[TILE_COUNT] = {NULL};
Point viewPosition = {0};

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

static TILE GetTileForCell(gint positionX, gint positionY);

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

            guint tileIndex = (guint)GetTileForCell(originPos->x + x, originPos->y + y);
            gtk_image_set_from_pixbuf(viewPieces[index], tiles[tileIndex]);
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
// Returns the index of the tiles array to enter the viewPiece based on the dungeonCell's terrain.
static TILE GetTileForCell(gint positionX, gint positionY)
{
    switch (GetCellTerrain(positionX, positionY))
    {
    case TERRAIN_FLOOR:
        return TILE_FLOOR_BASE;
    case TERRAIN_WALL:
        return GetWallTile(positionX, positionY);
    case TERRAIN_NULL:
        return TILE_NULL;
    }

    return TILE_NULL;
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
