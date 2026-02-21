#ifndef MENU_BOX
#define MENU_BOX

#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include "viewPort.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------

#ifdef KINDLE_BUILD
#define MENU_BOX_WIDTH  VIEWPORT_WIDTH
#define MENU_BOX_HEIGHT 736
#else
#define MENU_BOX_WIDTH  VIEWPORT_WIDTH
#define MENU_BOX_HEIGHT 368
#endif

// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------

enum MenuState
{
    STATE_INSPECT,
    STATE_CHARACTER,
    STATE_INVENTORY,
    STATE_LOGBOOK,
    STATE_SETTINGS,
    STATE_COUNT
};

// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

extern GtkDrawingArea *menuBox;
extern MenuState menuState;

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

void InitMenuBox(void);
MenuState GetMenuState(void);
void SetMenuState(MenuState state);
gboolean on_menuBox_update(GtkWidget *widget, cairo_t *context, gpointer userData);
gboolean on_menuBox_click(GtkWidget *widget, GdkEventButton *event, gpointer userData);

#endif // MENU_BOX
