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

const gint8 hMovement[DIR_ALL_COUNT] = { 0, 1, 0, -1,  1, 1, -1, -1};
const gint8 vMovement[DIR_ALL_COUNT] = {-1, 0, 1,  0, -1, 1,  1, -1};

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
    case DIR_EAST:
        return DIR_WEST;
    case DIR_SOUTH:
        return DIR_NORTH;
    case DIR_WEST:
        return DIR_EAST;
    case DIR_NORTH_EAST:
        return DIR_SOUTH_WEST;
    case DIR_SOUTH_EAST:
        return DIR_NORTH_WEST;
    case DIR_SOUTH_WEST:
        return DIR_NORTH_EAST;
    case DIR_NORTH_WEST:
        return DIR_SOUTH_EAST;
    default:
        return DIR_NONE;
    }
}

//-------------------------------------------------------------------------------------------------
// Returns TRUE if the gven value is even and FALSE if it is odd.
gboolean IsValueEven(gint value)
{
    if (value % 2 == 0)
        return TRUE;
    else
        return FALSE;
}

//-------------------------------------------------------------------------------------------------
// Returns the given value if within the given range (inclusive). Otherwise, returns the closest
// boundary of the given range.
gint ClampValue(gint value, gint minimum, gint maximum)
{
    if (value < minimum)
        return minimum;
    else if (value > maximum)
        return maximum;
    else
        return value;
}

//-------------------------------------------------------------------------------------------------
// Returns the given value if within the given range (inclusive). Otherwise, wraps the value around
// the range by the difference between the closest bound.
gint WrapValue(gint value, gint minimum, gint maximum)
{
    if (value < minimum)
        return (maximum - (minimum - value)) + 1;
    else if (value > maximum)
        return (minimum + (value - maximum)) - 1;
    else
        return value;
}

// ------------------------------------------------------------------------------------------------
// Returns TRUE if the given position is with the given rectangle.
gboolean IsWithinRectangle(Point *position, Point *origin, gint width, gint height)
{
    if (position->x > origin->x && position->x < origin->x + width
      && position->y > origin->y && position->y < origin->y + height)
        return TRUE;
    else
        return FALSE;
}
