#ifndef PLAYER_CONTROLLER
#define PLAYER_CONTROLLER

#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------


#ifdef KINDLE_BUILD
#define ICON_SIZE 128
#define CONTROLS_BOX_WIDTH  1000
#define CONTROLS_BOX_HEIGHT 400
#else
#define ICON_SIZE 32
#define CONTROLS_BOX_WIDTH  750
#define CONTROLS_BOX_HEIGHT 256
#endif

#define ARROWS_START_X 0
#define ARROWS_START_Y 0
#define ARROWS_WIDTH   ICON_SIZE * 3
#define ARROWS_HEIGHT  ARROWS_WIDTH

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
