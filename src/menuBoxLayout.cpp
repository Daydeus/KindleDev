#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include "global.h"
#include "tile.h"
#include "menuBox.h"
#include "menuBoxLayout.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------

// menuBox characterItem positions.
#define TERRAIN_FLIP_BTTN_X     TILE_SIZE_MB * 3/2
#define TERRAIN_FLIP_BTTN_Y     TILE_SIZE_MB / 2
#define TERRAIN_FLIP_TEXT_X     TERRAIN_FLIP_BTTN_X + (TILE_SIZE_MB * 3/2)
#define TERRAIN_FLIP_TEXT_Y     TERRAIN_FLIP_BTTN_Y + (TILE_SIZE_MB / 4)

// menuBox settingsItem positions.
#define ZOOM_SWITCH_X      TILE_SIZE_MB * 3/2
#define ZOOM_SWITCH_Y      TILE_SIZE_MB / 2
#define ZOOM_TEXT_X        ZOOM_SWITCH_X + (TILE_SIZE_MB * 3/2)
#define ZOOM_TEXT_Y        ZOOM_SWITCH_Y

#define REFRESH_BTTN_X     ZOOM_SWITCH_X
#define REFRESH_BTTN_Y     ZOOM_SWITCH_Y + TILE_SIZE_MB
#define REFRESH_TEXT_X     REFRESH_BTTN_X + (TILE_SIZE_MB * 3/2)
#define REFRESH_TEXT_Y     ZOOM_SWITCH_Y + TILE_SIZE_MB

#define FOGOFWAR_BTTN_X    ZOOM_SWITCH_X
#define FOGOFWAR_BTTN_Y    REFRESH_BTTN_Y + TILE_SIZE_MB
#define FOGOFWAR_TEXT_X    REFRESH_BTTN_X + (TILE_SIZE_MB * 3/2)
#define FOGOFWAR_TEXT_Y    REFRESH_TEXT_Y + TILE_SIZE_MB

#define EXIT_TEXT_X        MENU_BOX_WIDTH - TILE_SIZE_MB * 7/2
#define EXIT_TEXT_Y        MENU_BOX_HEIGHT - TILE_SIZE_MB * 2

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
    { {{ZOOM_SWITCH_X,   ZOOM_SWITCH_Y},   TILE_SIZE_MB, TILE_SIZE_MB}, FALSE}, // ZOOM_SWITCH
    { {{ZOOM_TEXT_X,     ZOOM_TEXT_Y},     TILE_SIZE_MB, TILE_SIZE_MB}, TRUE},  // ZOOM_TEXT
    { {{REFRESH_BTTN_X,  REFRESH_BTTN_Y},  TILE_SIZE_MB, TILE_SIZE_MB}, FALSE}, // REFRESH_BUTTON
    { {{REFRESH_TEXT_X,  REFRESH_TEXT_Y},  TILE_SIZE_MB, TILE_SIZE_MB}, TRUE},  // REFRESH_TEXT
    { {{FOGOFWAR_BTTN_X, FOGOFWAR_BTTN_Y}, TILE_SIZE_MB, TILE_SIZE_MB}, FALSE}, // FOGOFWAR_BUTTON
    { {{FOGOFWAR_TEXT_X, FOGOFWAR_TEXT_Y}, TILE_SIZE_MB, TILE_SIZE_MB}, TRUE},  // FOGOFWAR_TEXT
    { {{EXIT_TEXT_X,     EXIT_TEXT_Y},     TILE_SIZE_MB, TILE_SIZE_MB}, TRUE},  // EXIT_TEXT
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
    case MB_SETTINGS_REFRESH_TEXT:
        return "Regenerate";
    case MB_SETTINGS_FOGOFWAR_TEXT:
        return "Fog of War";
    case MB_SETTINGS_EXIT_TEXT:
        return "Exit";
    default:
        return NULL;
    }
}
