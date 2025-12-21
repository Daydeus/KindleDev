#ifndef MENUBOX_LAYOUT
#define MENUBOX_LAYOUT

#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include "global.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------

enum SettingsUI
{
    SETTINGS_ZOOM_SWITCH,
    SETTINGS_ZOOM_TEXT,
    SETTINGS_REFRESH_BUTTON,
    SETTINGS_REFRESH_TEXT,
    SETTINGS_EXIT_TEXT,
    SETTINGS_COUNT
};

struct MenuLayout
{
    Rectangle layout;
    guint isText :1;
};

// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

extern MenuLayout settingsItems[SETTINGS_COUNT];

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

MenuLayout* GetSettingsLayoutItem(SettingsUI item);
const char* GetSettingsLayoutText(SettingsUI item);

#endif // MENUBOX_LAYOUT
