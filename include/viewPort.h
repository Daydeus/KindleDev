#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------

#ifdef KINDLE_BUILD
#define TILE_SIZE 32
#else
#define TILE_SIZE 16
#endif

#define VIEWPORT_WIDTH_TILES  39
#define VIEWPORT_HEIGHT_TILES 25
#define VIEWPORT_WIDTH_PIXELS  VIEWPORT_WIDTH_TILES * TILE_SIZE
#define VIEWPORT_HEIGHT_PIXELS VIEWPORT_HEIGHT_TILES * TILE_SIZE

// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------

// Indices for tiles in the GdkPixbuf tiles array.
enum Tile
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

extern GdkPixbuf *tiles[TILE_COUNT];
extern GtkDrawingArea *viewPort;
extern ZoomLevel zoomLevel;
extern Point viewPosition;
extern Point selectedCell;

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

void InitViewPort(void);
Point* GetViewPosition(void);
void SetViewPosition(gint positionX, gint positionY);
void MoveViewPosition(Direction direction, guint distance);
void CenterViewPortOn(gint positionX, gint positionY);
Point* GetSelectedCell(void);
void SetSelectedCell(gint positionX, gint positionY);
void LoadTiles(void);
void FreeTiles(void);
ZoomLevel GetViewPortZoomLevel(void);
void SetViewPortZoomLevel(ZoomLevel level);
gboolean on_viewPort_update(GtkWidget *widget, cairo_t *context, gpointer userData);
gboolean on_viewPort_click(GtkWidget *widget, GdkEventButton *event, gpointer userData);

#endif // VIEWPORT_H
