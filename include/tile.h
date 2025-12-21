#ifndef TILE_H
#define TILE_H

#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include "menuBoxLayout.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------

#define TILE_SIZE_16 16
#define TILE_SIZE_32 32
#define TILE_SIZE_64 64

#define TILESET_WIDTH 8

#ifdef KINDLE_BUILD
#define TILE_SIZE_MB     TILE_SIZE_64
#define TILE_SIZE_BORDER TILE_SIZE_64
#else
#define TILE_SIZE_MB     TILE_SIZE_32
#define TILE_SIZE_BORDER TILE_SIZE_32
#endif

// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------

// Colors available for the application to use.
enum Color
{
    COLOR_BLACK,
    COLOR_GREY_DARK,
    COLOR_GREY_LIGHT,
    COLOR_WHITE,
    COLOR_COUNT_SOLID,
    COLOR_DITHER_1 = COLOR_COUNT_SOLID,
    COLOR_DITHER_2,
    COLOR_DITHER_3,
    COLOR_DITHER_4,
    COLOR_COUNT_ALL
};

// Indices for GdkPixbufs in the dungeonTiles array.
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
    TILE_COUNT_VP
};

// Indices for GdkPixbufs in the menuBoxTiles array.
enum MenuBoxTile
{
    TILE_ARROW_NORTH,
    TILE_ARROW_EAST,
    TILE_ARROW_SOUTH,
    TILE_ARROW_WEST,
    TILE_UI_SWITCH_OFF,
    TILE_UI_SWITCH_MID,
    TILE_UI_SWITCH_ON,
    TILE_REFRESH,
    TILE_COUNT_MB
};

// Indices for GdkPixbufs in the b array.
enum BorderTile
{
    TILE_BORDER_NORTH_1,
    TILE_BORDER_NORTH_2,
    TILE_BORDER_NORTH_3,
    TILE_BORDER_EAST_1,
    TILE_BORDER_EAST_2,
    TILE_BORDER_EAST_3,
    TILE_BORDER_SOUTH_1,
    TILE_BORDER_SOUTH_2,
    TILE_BORDER_SOUTH_3,
    TILE_BORDER_WEST_1,
    TILE_BORDER_WEST_2,
    TILE_BORDER_WEST_3,
    TILE_BORDER_CORNER_NORTH_EAST,
    TILE_BORDER_CORNER_SOUTH_EAST,
    TILE_BORDER_CORNER_SOUTH_WEST,
    TILE_BORDER_CORNER_NORTH_WEST,
    TILE_BORDER_INSPECT_OFF,
    TILE_BORDER_ICON_OFF_OFFSET = TILE_BORDER_INSPECT_OFF,
    TILE_BORDER_CHARACTER_OFF,
    TILE_BORDER_INVENTORY_OFF,
    TILE_BORDER_LOGBOOK_OFF,
    TILE_BORDER_SETTINGS_OFF,
    TILE_BORDER_INSPECT_ON,
    TILE_BORDER_ICON_ON_OFFSET = TILE_BORDER_INSPECT_ON,
    TILE_BORDER_CHARACTER_ON,
    TILE_BORDER_INVENTORY_ON,
    TILE_BORDER_LOGBOOK_ON,
    TILE_BORDER_SETTINGS_ON,
    TILE_COUNT_BORDER
};

enum DungeonTileset
{
    TILESET_CAVE,
    TILESET_COUNT
};

// Bitmask for determining which wall tile to display.
enum TileMask
{
    MASK_NORTH_EAST = 1,
    MASK_SOUTH_EAST = 2,
    MASK_SOUTH_WEST = 4,
    MASK_NORTH_WEST = 8,
    MASK_COUNT
};

// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

extern GdkPixbuf *colorFillTiles[COLOR_COUNT_ALL];
extern GdkPixbuf *borderTiles[TILE_COUNT_BORDER];
extern GdkPixbuf *dungeonTiles[TILE_COUNT_VP];
extern GdkPixbuf *menuBoxTiles[TILE_COUNT_MB];

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

void LoadColorFillTiles(void);
void FreeColorFillTiles(void);
void LoadBorderTiles(void);
void FreeBorderTiles(void);
void LoadDungeonTiles(DungeonTileset tileset);
void FreeDungeonTiles(void);
GdkPixbuf* GetTileForActor(guint actorIndex);
GdkPixbuf* GetTileForTerrain(Point *position);
GdkPixbuf* GetTileForMenuBoxSettings(SettingsUI item);
guint GetTileSizeForZoom(gboolean zoomIsOn);
void ScaleTileForZoom(gboolean zoomIsOn);
void LoadMenuBoxTiles(void);
void FreeMenuBoxTiles(void);
void SetWidgetBgColor(GtkWidget *widget, enum Color colorName);
void FillColorRectangle(cairo_t *context, Point *origin, gint width, gint height, enum Color color);

#endif // TILE_H
