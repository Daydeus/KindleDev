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
#else
#define ICON_SIZE 32
#endif

#define CONTROLS_BOX_WIDTH  VIEWPORT_WIDTH_PIXELS
#define CONTROLS_BOX_HEIGHT VIEWPORT_HEIGHT_PIXELS

#define ARROWS_ORIGIN_X 0
#define ARROWS_ORIGIN_Y 0
#define ARROWS_WIDTH   ICON_SIZE * 3
#define ARROWS_HEIGHT  ARROWS_WIDTH

#define ZOOM_ORIGIN_X 400
#define ZOOM_ORIGIN_Y 0

#define EXIT_ORIGIN_X 400
#define EXIT_ORIGIN_Y 128

// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------

enum Icon
{
    ICON_ARROW_NORTH,
    ICON_ARROW_NORTH_EAST,
    ICON_ARROW_EAST,
    ICON_ARROW_SOUTH_EAST,
    ICON_ARROW_SOUTH,
    ICON_ARROW_SOUTH_WEST,
    ICON_ARROW_WEST,
    ICON_ARROW_NORTH_WEST,
    ICON_SETTING_OFF,
    ICON_SETTING_MID,
    ICON_SETTING_ON,
    ICON_SETTING_EXIT,
    ICON_COUNT
};

// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

extern GdkPixbuf *icons[ICON_COUNT];
extern GtkDrawingArea *playerControlsBox;

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

void InitControlsBox(void);
void LoadIcons(void);
void FreeIcons(void);
gboolean on_playerControlsBox_update(GtkWidget *widget, cairo_t *context, gpointer userData);
gboolean on_playerControlsBox_click(GtkWidget *widget, GdkEventButton *event, gpointer userData);

#endif // PLAYER_CONTROLLER
