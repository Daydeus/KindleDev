#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------

#define VIEWPORT_WIDTH  25
#define VIEWPORT_HEIGHT 15
#define SCALE_SIZE 48

// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------

enum TILE
{
    TILE_NULL,
    TILE_WALL_TOP,
    TILE_WALL_RIGHT,
    TILE_WALL_TOP_RIGHT,
    TILE_WALL_BOTTOM,
    TILE_WALL_LEFT,
    TILE_WALL_TOP_LEFT,
    TILE_WALL_RIGHT_LEFT,
    TILE_WALL_TOP_RIGHT_LEFT,
    TILE_FLOOR_BASE,
    TILE_CELL_SELECTED,
    TILE_COUNT
};

enum TILE_MASK
{
    MASK_NONE,
    MASK_TOP,
    MASK_RIGHT,
    MASK_TOP_RIGHT,
    MASK_BOTTOM,
    MASK_TOP_BOTTOM,
    MASK_RIGHT_BOTTOM,
    MASK_TOP_RIGHT_BOTTOM,
    MASK_LEFT,
    MASK_TOP_LEFT,
    MASK_RIGHT_LEFT,
    MASK_TOP_RIGHT_LEFT,
    MASK_BOTTOM_LEFT,
    MASK_TOP_BOTTOM_LEFT,
    MASK_RIGHT_BOTTOM_LEFT,
    MASK_TOP_RIGHT_BOTTOM_LEFT,
    MASK_COUNT
};

enum DIRECTION
{
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT,
    DIR_COUNT
};

// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

extern GdkPixbuf *tiles[TILE_COUNT];
extern Point viewPosition;
extern Point selectedCell;

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

Point* GetViewPosition(void);
void SetViewPosition(gint positionX, gint positionY);
void MoveViewPosition(DIRECTION direction, guint distance);
void CenterViewPositionOn(gint positionX, gint positionY);
Point* GetSelectedCell(void);
void SetSelectedCell(gint positionX, gint positionY);
const guint8* GetTileData(enum TILE tile);
void LoadImagesToPixbufs(void);
void FreePixbufs(void);

#endif // VIEWPORT_H
