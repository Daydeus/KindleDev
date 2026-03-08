#ifndef MENU_LAYOUT
#define MENU_LAYOUT

#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include "global.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------

#define MENU_TABS_TOP       (TILE_SIZE_BORDER * 4)
#define MENU_TABS_BOTTOM    (MENU_TABS_TOP + TILE_SIZE_MB + TILE_SIZE_BORDER)
#define MENU_CORNER_X       (TILE_SIZE_MB * 3/4)
#define MENU_CORNER_Y       MENU_TABS_BOTTOM

// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------

enum ActionsUI
{
    MB_ACTIONS_TERRAIN_FLIP_BTTN,
    MB_ACTIONS_TERRAIN_FLIP_TEXT,
    MB_ACTIONS_COUNT
};

enum SettingsUI
{
    MB_SETTINGS_SKIP_TEXT,
    MB_SETTINGS_SKIP_BUTTON,
    MB_SETTINGS_FOGOFWAR_TEXT,
    MB_SETTINGS_FOGOFWAR_BUTTON,
    MB_SETTINGS_EXIT_TEXT,
    MB_SETTINGS_EXIT_BUTTON,
    MB_SETTINGS_COUNT
};

struct MenuLayout
{
    Rectangle layout;
    guint isText :1;
};

// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

extern MenuLayout characterItems[MB_ACTIONS_COUNT];
extern MenuLayout settingsItems[MB_SETTINGS_COUNT];

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

MenuLayout* GetActionsLayoutItem(ActionsUI item);
const char* GetActionsLayoutText(ActionsUI item);
MenuLayout* GetSettingsLayoutItem(SettingsUI item);
const char* GetSettingsLayoutText(SettingsUI item);

#endif // MENU_LAYOUT
