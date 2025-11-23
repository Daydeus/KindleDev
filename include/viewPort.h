#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include "global.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------

#if KINDLE_BUILD
#define VIEWPORT_WIDTH_PIXELS  1216
#define VIEWPORT_HEIGHT_PIXELS 832
#else
#define VIEWPORT_WIDTH_PIXELS  704
#define VIEWPORT_HEIGHT_PIXELS 448
#endif

#define TILE_SIZE_16 16
#define TILE_SIZE_32 32
#define TILE_SIZE_64 64

// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------

// Indices for tiles in the GdkPixbuf tiles array.
enum DungeonTile
{
    TILE_NULL,
    TILE_WALL_INNER_CORNER_NORTHEAST,
    TILE_WALL_INNER_CORNER_SOUTHEAST,
    TILE_WALL_FACING_EAST,
    TILE_WALL_INNER_CORNER_SOUTHWEST,
    TILE_WALL_DUAL_CORNERS_SOUTHWEST_NORTHEAST,
    TILE_WALL_FACING_SOUTH,
    TILE_WALL_OUTER_CORNER_SOUTHEAST,
    TILE_WALL_INNER_CORNER_NORTHWEST,
    TILE_WALL_FACING_NORTH,
    TILE_WALL_DUAL_CORNERS_NORTHWEST_SOUTHEAST,
    TILE_WALL_OUTER_CORNER_NORTHEAST,
    TILE_WALL_FACING_WEST,
    TILE_WALL_OUTER_CORNER_NORTHWEST,
    TILE_WALL_OUTER_CORNER_SOUTHWEST,
    TILE_WALL_STANDALONE,
    TILE_FLOOR,
    TILE_AT,
    TILE_CELL_SELECTED,
    TILE_COUNT
};

enum TileMask
{
    MASK_NORTH_EAST = 1,
    MASK_SOUTH_EAST = 2,
    MASK_SOUTH_WEST = 4,
    MASK_NORTH_WEST = 8,
    MASK_COUNT
};

// Zoom Levels for the viewPort.
enum ZoomLevel
{
    ZOOM_LEVEL_OFF,
    ZOOM_LEVEL_MID,
    ZOOM_LEVEL_PEAK,
    ZOOM_LEVEL_COUNT
};

// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

extern GdkPixbuf *dungeonTiles[TILE_COUNT];
extern GtkDrawingArea *viewPort;
extern ZoomLevel zoomLevel;
extern Point viewPosition;
extern Point selectedCell;

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

void InitViewPort(void);
Point* GetViewPosition(void);
void SetViewPosition(Point *position);
void MoveViewPosition(Direction direction, guint distance);
void CenterViewPortOn(Point *position);
Point* GetSelectedCell(void);
void SetSelectedCell(Point *position);
void LoadDungeonTiles(void);
void ScaleTileForZoom(void);
void FreeDungeonTiles(void);
ZoomLevel GetViewPortZoomLevel(void);
void SetViewPortZoomLevel(ZoomLevel level);
gboolean on_viewPort_update(GtkWidget *widget, cairo_t *context, gpointer userData);

#endif // VIEWPORT_H
