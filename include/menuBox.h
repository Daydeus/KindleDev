#ifndef PLAYER_CONTROLLER
#define PLAYER_CONTROLLER

#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include "viewPort.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------

#ifdef KINDLE_BUILD
#define TILE_SIZE_MB 64
#define MENU_BOX_WIDTH  VIEWPORT_WIDTH_PIXELS
#define MENU_BOX_HEIGHT 720
#else
#define TILE_SIZE_MB 32
#define MENU_BOX_WIDTH  VIEWPORT_WIDTH_PIXELS
#define MENU_BOX_HEIGHT VIEWPORT_HEIGHT_PIXELS
#endif

#define ARROWS_CENTER_X   128
#define ARROWS_CENTER_Y   128
#define ARROWS_TILE_INDEX 8

#define ZOOM_ORIGIN_X 400
#define ZOOM_ORIGIN_Y 128

#define EXIT_ORIGIN_X 500
#define EXIT_ORIGIN_Y 128

// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------

enum MenuBoxTile
{
    TILE_BORDER_NORTH,
    TILE_BORDER_EAST,
    TILE_BORDER_SOUTH,
    TILE_BORDER_WEST,
    TILE_BORDER_CORNER_NORTH_EAST,
    TILE_BORDER_CORNER_SOUTH_EAST,
    TILE_BORDER_CORNER_SOUTH_WEST,
    TILE_BORDER_CORNER_NORTH_WEST,
    TILE_ARROW_NORTH,
    TILE_ARROW_EAST,
    TILE_ARROW_SOUTH,
    TILE_ARROW_WEST,
    TILE_UI_SWITCH_OFF,
    TILE_UI_SWITCH_MID,
    TILE_UI_SWITCH_ON,
    TILE_SETTING_EXIT,
    TILE_COUNT_MB
};

// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

extern GdkPixbuf *menuBoxTiles[TILE_COUNT_MB];
extern GtkDrawingArea *menuBox;

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

void InitControlsBox(void);
void LoadMenuBoxTiles(void);
void FreeMenuBoxTiles(void);
gboolean on_menuBox_update(GtkWidget *widget, cairo_t *context, gpointer userData);
gboolean on_menuBox_click(GtkWidget *widget, GdkEventButton *event, gpointer userData);

#endif // PLAYER_CONTROLLER
