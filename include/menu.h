#ifndef MENU_H
#define MENU_H

#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include "viewPort.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------

#define MENU_WIDTH   (608 * KINDLE_SCALE)
#define MENU_HEIGHT  (368 * KINDLE_SCALE)

// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------

enum MenuState
{
    STATE_INSPECT,
    STATE_CHARACTER,
    STATE_INVENTORY,
    STATE_LOGBOOK,
    STATE_MINIMAP,
    STATE_SETTINGS,
    STATE_COUNT
};

// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

extern GtkDrawingArea *menu;
extern MenuState menuState;

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

void InitMenu(void);
MenuState GetMenuState(void);
void SetMenuState(MenuState state);
gboolean on_menu_update(GtkWidget *widget, cairo_t *context, gpointer userData);
gboolean on_menu_click(GtkWidget *widget, GdkEventButton *event, gpointer userData);

#endif // MENU_H
