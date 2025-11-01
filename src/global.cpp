#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include "global.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

const gint8 hMovement[DIR_COUNT] = {0,  0,  1, 1, 1, 0, -1, -1, -1};
const gint8 vMovement[DIR_COUNT] = {0, -1, -1, 0, 1, 1,  1,  0, -1};

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Sets the widget's background color.
void SetWidgetBgColor(GtkWidget *widget, enum Color colorName)
{
    const gchar* string;
    GdkColor color;

    switch (colorName)
    {
    case COLOR_BLACK:
        string = "#000000";
    break;
    case COLOR_GREY_DARK:
        string = "#444444";
    break;
    case COLOR_GREY_LIGHT:
        string = "#bbbbbb";
    break;
    case COLOR_WHITE:
        string = "#ffffff";
    break;
    }

    if (gdk_color_parse(string, &color))
    {
        gtk_widget_modify_bg(widget, GTK_STATE_NORMAL, &color);
    }
}
