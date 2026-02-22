#ifndef TILE_H
#define TILE_H

#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include "menuLayout.h"
#include "actor.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------

#define TILE_SIZE_16 16
#define TILE_SIZE_32 32
#define TILE_SIZE_64 64

#define TILESET_WIDTH 8

#ifdef KINDLE_BUILD
#define TILE_SIZE_MB     TILE_SIZE_64
#define TILE_SIZE_BORDER TILE_SIZE_32
#else
#define TILE_SIZE_MB     TILE_SIZE_32
#define TILE_SIZE_BORDER TILE_SIZE_16
#endif

// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------

// Indices for GdkPixbufs in the actorTiles array.
enum ActorTile
{
    TILE_ACTOR_SELECTOR_UNLOCKED,
    TILE_ACTOR_SELECTOR_LOCKED,
    TILE_ACTOR_PLAYER_LEFT,
    TILE_ACTOR_PLAYER_RIGHT,
    TILE_ACTOR_SLIME_LEFT,
    TILE_ACTOR_SLIME_RIGHT,
    TILE_ACTOR_DUMMY,
    TILE_ACTOR_COUNT
};

// Indices for GdkPixbufs in the borderTiles array.
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
    TILE_BORDER_T_NORTH,
    TILE_BORDER_T_EAST,
    TILE_BORDER_T_SOUTH,
    TILE_BORDER_T_WEST,
    TILE_BORDER_MIDDLE_H_1,
    TILE_BORDER_MIDDLE_H_2,
    TILE_BORDER_MIDDLE_H_3,
    TILE_BORDER_MIDDLE_V_1,
    TILE_BORDER_MIDDLE_V_2,
    TILE_BORDER_MIDDLE_V_3,
    TILE_BORDER_COUNT
};

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

// Indices for GdkPixbufs in the terrain(Light/Dark)Tiles array.
enum TerrainTile
{
    TILE_TERRAIN_EDGE,
    TILE_TERRAIN_WALL_INNER_CORNER_NE,
    TILE_TERRAIN_WALL_INNER_CORNER_SE,
    TILE_TERRAIN_WALL_FACING_EAST,
    TILE_TERRAIN_WALL_INNER_CORNER_SW,
    TILE_TERRAIN_WALL_DUAL_CORNERS_SW_NE,
    TILE_TERRAIN_WALL_FACING_SOUTH,
    TILE_TERRAIN_WALL_OUTER_CORNER_SE,
    TILE_TERRAIN_WALL_INNER_CORNER_NW,
    TILE_TERRAIN_WALL_FACING_NORTH,
    TILE_TERRAIN_WALL_DUAL_CORNERS_NW_SE,
    TILE_TERRAIN_WALL_OUTER_CORNER_NE,
    TILE_TERRAIN_WALL_FACING_WEST,
    TILE_TERRAIN_WALL_OUTER_CORNER_NW,
    TILE_TERRAIN_WALL_OUTER_CORNER_SW,
    TILE_TERRAIN_WALL_STANDALONE,
    TILE_TERRAIN_FLOOR,
    TILE_TERRAIN_STAIRS,
    TILE_TERRAIN_COUNT
};

// Indices for GdkPixbufs in the menuTiles array.
enum MenuTile
{
    TILE_MENU_EXIT,
    TILE_MENU_REFRESH,
    TILE_MENU_UI_SWITCH_OFF,
    TILE_MENU_UI_SWITCH_MID,
    TILE_MENU_UI_SWITCH_ON,
    TILE_MENU_BAR_END_LEFT,
    TILE_MENU_BAR_MIDDLE,
    TILE_MENU_BAR_END_RIGHT,
    TILE_MENU_HEART,
    TILE_MENU_INSPECT_OFF,
    TILE_MENU_CHARACTER_OFF,
    TILE_MENU_INVENTORY_OFF,
    TILE_MENU_LOGBOOK_OFF,
    TILE_MENU_SETTINGS_OFF,
    TILE_MENU_INSPECT_ON,
    TILE_MENU_CHARACTER_ON,
    TILE_MENU_INVENTORY_ON,
    TILE_MENU_LOGBOOK_ON,
    TILE_MENU_SETTINGS_ON,
    TILE_MENU_COUNT
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

extern GdkPixbuf *actorTiles[TILE_ACTOR_COUNT];
extern GdkPixbuf *borderTiles[TILE_BORDER_COUNT];
extern GdkPixbuf *colorFillTiles[COLOR_COUNT_ALL];
extern GdkPixbuf *menuTiles[TILE_MENU_COUNT];
extern GdkPixbuf *terrainDarkTiles[TILE_TERRAIN_COUNT];
extern GdkPixbuf *terrainLightTiles[TILE_TERRAIN_COUNT];

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

void LoadActorTiles(void);
void FreeActorTiles(void);
GdkPixbuf* GetTileForActor(Actor *actor);
GdkPixbuf* GetTileForCellSelector(void);
void LoadBorderTiles(void);
void FreeBorderTiles(void);
void LoadColorFillTiles(void);
void FreeColorFillTiles(void);
void FillColorRectangle(cairo_t *context, Point *origin, gint width, gint height, enum Color color);
void SetWidgetBgColor(GtkWidget *widget, enum Color colorName);
void LoadTerrainTiles(void);
void FreeTerrainTiles(void);
GdkPixbuf* GetTileForTerrain(Point *position);
void LoadMenuTiles(void);
void FreeMenuTiles(void);
GdkPixbuf* GetTileForMenuState(MenuState state);
GdkPixbuf* GetTileForMenuCharacter(CharacterUI item);
GdkPixbuf* GetTileForMenuSettings(SettingsUI item);
guint GetTileSizeForZoom(gboolean zoomIsOn);
void ScaleTileForZoom(gboolean zoomIsOn);

#endif // TILE_H
