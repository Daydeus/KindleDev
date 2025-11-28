#ifndef PLAYER_CONTROLLER
#define PLAYER_CONTROLLER

#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include "viewPort.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------

#ifdef KINDLE_BUILD
#define MENU_BOX_WIDTH  VIEWPORT_WIDTH
#define MENU_BOX_HEIGHT 720
#else
#define MENU_BOX_WIDTH  VIEWPORT_WIDTH
#define MENU_BOX_HEIGHT VIEWPORT_HEIGHT
#endif

#define ARROWS_CENTER_X   128
#define ARROWS_CENTER_Y   128

#define ZOOM_ORIGIN_X 400
#define ZOOM_ORIGIN_Y 128

#define EXIT_ORIGIN_X 500
#define EXIT_ORIGIN_Y 128

// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

extern GtkDrawingArea *menuBox;

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

void InitMenuBox(void);
gboolean on_menuBox_update(GtkWidget *widget, cairo_t *context, gpointer userData);
gboolean on_menuBox_click(GtkWidget *widget, GdkEventButton *event, gpointer userData);

#endif // PLAYER_CONTROLLER
