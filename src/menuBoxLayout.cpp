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
#define ZOOM_SWITCH_X  TILE_SIZE_MB * 3/2
#define ZOOM_SWITCH_Y  TILE_SIZE_MB * 3/2
#define ZOOM_TEXT_X    ZOOM_SWITCH_X + (TILE_SIZE_MB * 3/2)
#define ZOOM_TEXT_Y    ZOOM_SWITCH_Y
#define EXIT_TEXT_X    MENU_BOX_WIDTH - TILE_SIZE_MB * 7/2
#define EXIT_TEXT_Y    MENU_BOX_HEIGHT - TILE_SIZE_MB * 2

// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

MenuLayout settingsItems[SETTINGS_COUNT] =
{   //  origin.x       origin.y        width         height         isText
    { {{ZOOM_SWITCH_X, ZOOM_SWITCH_Y}, TILE_SIZE_MB, TILE_SIZE_MB}, FALSE}, // SETTINGS_ZOOM_SWITCH
    { {{ZOOM_TEXT_X,   ZOOM_TEXT_Y},   TILE_SIZE_MB, TILE_SIZE_MB}, TRUE},  // SETTINGS_ZOOM_TEXT
    { {{EXIT_TEXT_X,   EXIT_TEXT_Y},   TILE_SIZE_MB, TILE_SIZE_MB}, TRUE},  // SETTINGS_EXIT_TEXT
};

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Gets the Layout for the given SettingsItem.
MenuLayout* GetSettingsLayoutItem(SettingsUI item)
{
    return &settingsItems[item];
}

// ------------------------------------------------------------------------------------------------
// Gets the text for the given SettingsItem.
const char* GetSettingsLayoutText(SettingsUI item)
{
    switch (item)
    {
    case SETTINGS_ZOOM_TEXT:
        return "Zoom";
    case SETTINGS_EXIT_TEXT:
        return "Exit";
    default:
        return NULL;
    }
}
