#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include "global.h"
#include "tile.h"
#include "menuBox.h"
#include "menuBoxLayout.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------

// menuBox settingsItem positions.
#define ZOOM_SWITCH_X (MENU_BOX_WIDTH - TILE_SIZE_MB * 7 / 2)
#define ZOOM_SWITCH_Y (TILE_SIZE_MB * 2)
#define EXIT_BUTTON_X (MENU_BOX_WIDTH - TILE_SIZE_MB * 7 / 2)
#define EXIT_BUTTON_Y (MENU_BOX_HEIGHT - TILE_SIZE_MB * 2)

// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

Rectangle settingsItems[SETTINGS_COUNT] =
{   // origin.x       origin.y        width         height
    { {ZOOM_SWITCH_X, ZOOM_SWITCH_Y}, TILE_SIZE_MB, TILE_SIZE_MB}, // SETTINGS_ZOOM_SWITCH
    { {EXIT_BUTTON_X, EXIT_BUTTON_Y}, TILE_SIZE_MB, TILE_SIZE_MB}, // SETTINGS_EXIT_BUTTON
};

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Gets the Layout for the given SettingsItem.
Rectangle* GetSettingsItemLayout(SettingsUI item)
{
    return &settingsItems[item];
}
