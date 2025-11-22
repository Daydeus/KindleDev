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

const gint8 hMovement[DIR_COUNT] = { 0,  1, 1, 1, 0, -1, -1, -1};
const gint8 vMovement[DIR_COUNT] = {-1, -1, 0, 1, 1,  1,  0, -1};

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Returns TRUE if the given direction is DIR_NORTH, DIR_EAST, DIR_SOUTH, or DIR_WEST.
gboolean IsCardinalDirection(Direction direction)
{
    switch (direction)
    {
    case DIR_NORTH:
    case DIR_EAST:
    case DIR_SOUTH:
    case DIR_WEST:
        return TRUE;
    default:
        return FALSE;
    }
}

// ------------------------------------------------------------------------------------------------
// Returns the direction opposite of the given direction.
Direction GetOppositeDirection(Direction direction)
{
    switch (direction)
    {
    case DIR_NORTH:
        return DIR_SOUTH;
    case DIR_NORTH_EAST:
        return DIR_SOUTH_WEST;
    case DIR_EAST:
        return DIR_WEST;
    case DIR_SOUTH_EAST:
        return DIR_NORTH_WEST;
    case DIR_SOUTH:
        return DIR_NORTH;
    case DIR_SOUTH_WEST:
        return DIR_NORTH_EAST;
    case DIR_WEST:
        return DIR_EAST;
    case DIR_NORTH_WEST:
        return DIR_SOUTH_EAST;
    default:
        return DIR_NONE;
    }
}

// ------------------------------------------------------------------------------------------------
// Returns if the given position is with the given rectangle.
gboolean IsWithinRectangle(Point *position, Point *origin, gint width, gint height)
{
    if (position->x > origin->x && position->x < origin->x + width
      && position->y > origin->y && position->y < origin->y + height)
        return TRUE;
    else
        return FALSE;
}

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
