#ifndef PLAYER_CONTROLLER
#define PLAYER_CONTROLLER

#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include "viewPort.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------

#ifdef KINDLE_BUILD
#define ICON_SIZE 64
#define MENU_BOX_WIDTH  VIEWPORT_WIDTH_PIXELS
#define MENU_BOX_HEIGHT 720
#else
#define ICON_SIZE 32
#define MENU_BOX_WIDTH  VIEWPORT_WIDTH_PIXELS
#define MENU_BOX_HEIGHT VIEWPORT_HEIGHT_PIXELS
#endif

#define ARROWS_CENTER_X   128
#define ARROWS_CENTER_Y   128
#define ARROWS_TILE_INDEX 0

#define ZOOM_ORIGIN_X 400
#define ZOOM_ORIGIN_Y 128

#define EXIT_ORIGIN_X 500
#define EXIT_ORIGIN_Y 128

// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------

enum Icon
{
    ICON_ARROW_NORTH,
    ICON_ARROW_EAST,
    ICON_ARROW_SOUTH,
    ICON_ARROW_WEST,
    ICON_SETTING_OFF,
    ICON_SETTING_MID,
    ICON_SETTING_ON,
    ICON_SETTING_EXIT,
    ICON_COUNT
};

// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

extern GdkPixbuf *menuBoxTiles[ICON_COUNT];
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
