#include <gtk-2.0/gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <glib-2.0/glib.h>
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

GtkTable *viewPort = NULL;
GtkImage *viewPieces[VIEWPORT_WIDTH * VIEWPORT_HEIGHT] = {NULL};
GtkEventBox *viewPieceEvents[VIEWPORT_WIDTH * VIEWPORT_HEIGHT] = {NULL};
GdkPixbuf *tiles[TILE_COUNT] = {NULL};
Point viewPosition = {0}; // The dungeonCell position of the viewPort origin.
Point selectedCell = {0}; // The current player-selected dungeonCell in the viewPort.

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

static TILE GetTileForCell(gint positionX, gint positionY);
static TILE GetTileForCellSelected(gint positionX, gint positionY);
static TILE GetTileForTerrain(gint positionX, gint positionY);

// ------------------------------------------------------------------------------------------------
// Initializes the GtkTable, GdkPixbufs, and GtkImages required for the viewPort to function.
void InitViewPort(void)
{
    // Initialize the viewPort GtkTable.
    viewPort = GTK_TABLE(gtk_table_new(VIEWPORT_HEIGHT, VIEWPORT_WIDTH, TRUE));

    // Load image data to Pixbufs.
   LoadImagesToPixbufs();

    // Initialize the viewPieces GtkImages, add them to the viewPieceEvents GtkEventBoxes
    // and attach them to the GtkTable viewPort.
    for (guint y = 0; y < VIEWPORT_HEIGHT; y++)
    {
        for (guint x = 0; x < VIEWPORT_WIDTH; x++)
        {
            guint index = (y * VIEWPORT_WIDTH) + x;

            viewPieces[index] = GTK_IMAGE(gtk_image_new());
            viewPieceEvents[index] = GTK_EVENT_BOX(gtk_event_box_new());

            gtk_container_add(GTK_CONTAINER(viewPieceEvents[index]), GTK_WIDGET(viewPieces[index]));
            gtk_table_attach(viewPort, GTK_WIDGET(viewPieceEvents[index]), x, x+1, y, y+1,
                             GTK_SHRINK, GTK_SHRINK, 0, 0);

            g_signal_connect(viewPieceEvents[index], "button_press_event", G_CALLBACK(on_viewPiece_pressed), NULL);
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
// Returns the index of the tiles array for the dungeonCell selected indicator.
static TILE GetTileForCellSelected(gint positionX, gint positionY)
{

    return TILE_CELL_SELECTED;
}

// ------------------------------------------------------------------------------------------------
// Returns the index of the tiles array for the given cell based on its terrain.
static TILE GetTileForTerrain(gint positionX, gint positionY)
{
    TERRAIN terrain = GetCellTerrain(positionX, positionY);

    switch (terrain)
    {
    case TERRAIN_FLOOR:
        return TILE_FLOOR_BASE;
    case TERRAIN_WALL:
        return GetWallTile(positionX, positionY);
    case TERRAIN_NULL:
        return TILE_NULL;
    }
}

// ------------------------------------------------------------------------------------------------
// Returns the index of the tile to enter in the viewPiece based on the dungeonCell's contents.
static TILE GetTileForCell(gint positionX, gint positionY)
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

    return TILE_NULL;
}

// ------------------------------------------------------------------------------------------------
// Sets the currently selected cell. If it is the same as the previously selected cell, centers the
// viewPort on it.
void on_viewPiece_pressed(GtkWidget *widget, gpointer callbackData)
{
    Point *oldSelected = GetSelectedCell();
    Point *viewPosition = GetViewPosition();
    Point newSelected = {0};
    guint viewPieceOffsetX = 0, viewPieceOffsetY = 0;

    // Get the viewPiece's offset from the viewPort origin.
    gtk_container_child_get(GTK_CONTAINER(viewPort), widget, "left-attach", &viewPieceOffsetX,
                            "top-attach", &viewPieceOffsetY, NULL);

    // Add the viewPosition and the offset together to get the selected dungeonCell.
    newSelected.x = viewPosition->x + viewPieceOffsetX;
    newSelected.y = viewPosition->y + viewPieceOffsetY;

    if (newSelected.x == oldSelected->x && newSelected.y == oldSelected->y)
        CenterViewPositionOn(newSelected.x, newSelected.y);

    SetSelectedCell(newSelected.x, newSelected.y);
    UpdateViewPieces();
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
