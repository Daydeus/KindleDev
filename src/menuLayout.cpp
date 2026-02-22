#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include "global.h"
#include "menu.h"
#include "menuLayout.h"
#include "tile.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------

#define MENU_CORNER_X           TILE_SIZE_MB * 3/4
#define MENU_CORNER_Y           MENU_TABS_BOTTOM

#ifdef KINDLE_BUILD
    #define TEXT_ICON_GAP_X     TILE_SIZE_MB * 4
    #define TEXT_ICON_GAP_Y     -TILE_SIZE_MB / 16
    #define MENU_ITEM_GAP_Y     TILE_SIZE_MB * 3/2
#else
    #define TEXT_ICON_GAP_X     TILE_SIZE_MB * 3
    #define TEXT_ICON_GAP_Y     -TILE_SIZE_MB / 4
    #define MENU_ITEM_GAP_Y     TILE_SIZE_MB
#endif

// menu characterItem positions.
#define TERRAIN_FLIP_TEXT_X     MENU_CORNER_X
#define TERRAIN_FLIP_TEXT_Y     MENU_CORNER_Y + TILE_SIZE_MB
#define TERRAIN_FLIP_BTTN_X     TERRAIN_FLIP_TEXT_X + TEXT_ICON_GAP_X
#define TERRAIN_FLIP_BTTN_Y     TERRAIN_FLIP_TEXT_Y + TEXT_ICON_GAP_Y

// menu settingsItem positions.
#define ZOOM_TEXT_X             MENU_CORNER_X
#define ZOOM_TEXT_Y             MENU_CORNER_Y + TILE_SIZE_MB
#define ZOOM_SWITCH_X           ZOOM_TEXT_X + TEXT_ICON_GAP_X
#define ZOOM_SWITCH_Y           ZOOM_TEXT_Y + TEXT_ICON_GAP_Y

#define SKIP_TEXT_X             ZOOM_TEXT_X
#define SKIP_TEXT_Y             ZOOM_TEXT_Y + MENU_ITEM_GAP_Y
#define SKIP_BTTN_X             ZOOM_SWITCH_X
#define SKIP_BTTN_Y             ZOOM_SWITCH_Y + MENU_ITEM_GAP_Y

#define FOGOFWAR_TEXT_X         ZOOM_TEXT_X
#define FOGOFWAR_TEXT_Y         SKIP_TEXT_Y + MENU_ITEM_GAP_Y
#define FOGOFWAR_BTTN_X         ZOOM_SWITCH_X
#define FOGOFWAR_BTTN_Y         SKIP_BTTN_Y + MENU_ITEM_GAP_Y

#define EXIT_TEXT_X             ZOOM_TEXT_X
#define EXIT_TEXT_Y             FOGOFWAR_TEXT_Y + MENU_ITEM_GAP_Y
#define EXIT_BTTN_X             ZOOM_SWITCH_X
#define EXIT_BTTN_Y             FOGOFWAR_BTTN_Y + MENU_ITEM_GAP_Y

// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

MenuLayout characterItems[MB_CHARACTER_COUNT] =
{   //  origin.x             origin.y              width         height         isText
    { {{TERRAIN_FLIP_BTTN_X, TERRAIN_FLIP_BTTN_Y}, TILE_SIZE_MB, TILE_SIZE_MB}, FALSE}, // TERRAIN_FLIP_BUTTON
    { {{TERRAIN_FLIP_TEXT_X, TERRAIN_FLIP_TEXT_Y}, TILE_SIZE_MB, TILE_SIZE_MB}, TRUE},  // TERRAIN_FLIP_TEXT
};

MenuLayout settingsItems[MB_SETTINGS_COUNT] =
{   //  origin.x         origin.y          width         height         isText
    { {{ZOOM_TEXT_X,     ZOOM_TEXT_Y},     TILE_SIZE_MB, TILE_SIZE_MB}, TRUE},  // ZOOM_TEXT
    { {{ZOOM_SWITCH_X,   ZOOM_SWITCH_Y},   TILE_SIZE_MB, TILE_SIZE_MB}, FALSE}, // ZOOM_SWITCH
    { {{SKIP_TEXT_X,     SKIP_TEXT_Y},     TILE_SIZE_MB, TILE_SIZE_MB}, TRUE},  // SKIP_TEXT
    { {{SKIP_BTTN_X,     SKIP_BTTN_Y},     TILE_SIZE_MB, TILE_SIZE_MB}, FALSE}, // SKIP_BUTTON
    { {{FOGOFWAR_TEXT_X, FOGOFWAR_TEXT_Y}, TILE_SIZE_MB, TILE_SIZE_MB}, TRUE},  // FOGOFWAR_TEXT
    { {{FOGOFWAR_BTTN_X, FOGOFWAR_BTTN_Y}, TILE_SIZE_MB, TILE_SIZE_MB}, FALSE}, // FOGOFWAR_BUTTON
    { {{EXIT_TEXT_X,     EXIT_TEXT_Y},     TILE_SIZE_MB, TILE_SIZE_MB}, TRUE},  // EXIT_TEXT
    { {{EXIT_BTTN_X,     EXIT_BTTN_Y},     TILE_SIZE_MB, TILE_SIZE_MB}, FALSE}, // EXIT_BUTTON
};

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Gets the Layout for the given characterItem.
MenuLayout* GetCharacterLayoutItem(CharacterUI item)
{
    return &characterItems[item];
}

// ------------------------------------------------------------------------------------------------
// Gets the text for the given characterItem.
const char* GetCharacterLayoutText(CharacterUI item)
{
    switch (item)
    {
    case MB_CHARACTER_TERRAIN_FLIP_TEXT:
        return "Terrain Flip";
    default:
        return NULL;
    }
}

// ------------------------------------------------------------------------------------------------
// Gets the Layout for the given settingsItem.
MenuLayout* GetSettingsLayoutItem(SettingsUI item)
{
    return &settingsItems[item];
}

// ------------------------------------------------------------------------------------------------
// Gets the text for the given settingsItem.
const char* GetSettingsLayoutText(SettingsUI item)
{
    switch (item)
    {
    case MB_SETTINGS_ZOOM_TEXT:
        return "Zoom";
    case MB_SETTINGS_SKIP_TEXT:
        return "Skip Floor";
    case MB_SETTINGS_FOGOFWAR_TEXT:
        return "Fog of War";
    case MB_SETTINGS_EXIT_TEXT:
        return "Exit";
    default:
        return NULL;
    }
}
