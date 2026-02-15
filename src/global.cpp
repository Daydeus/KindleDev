#include <cstdlib>
#include <math.h>
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

// ------------------------------------------------------------------------------------------------
// Returns a string for the unicode direction arrow.
const char* GetDirectionSymbol(Direction direction)
{
    switch (direction)
    {
    case DIR_NORTH:
        return "\u2191";
    case DIR_EAST:
        return "\u2192";
    case DIR_SOUTH:
        return "\u2193";
    case DIR_WEST:
        return "\u2190";
    case DIR_NORTH_EAST:
        return "\u2197";
    case DIR_SOUTH_EAST:
        return "\u2198";
    case DIR_SOUTH_WEST:
        return "\u2199";
    case DIR_NORTH_WEST:
        return "\u2196";
    case DIR_NONE:
        return ".";
    default:
        return "?";
    }
}

//-------------------------------------------------------------------------------------------------
// Returns TRUE if the two given points have the same x and y values.
gboolean IsSamePoint(Point *point1, Point *point2)
{
    if (point1->x == point2->x && point1->y == point2->y)
        return TRUE;
    else
        return FALSE;
}

// ------------------------------------------------------------------------------------------------
// On a line drawn using Bresenham's algorithm from the given start position to the given end
// position, returns the next point along the line.
// Note: Use this function in a loop to perform an action on each point in the line.
Point* GetNextPointOnLine(Point *startPos, Point *endPos)
{
    gint changeX =  ABS(endPos->x - startPos->x);
    gint changeY = -ABS(endPos->y - startPos->y);

    gint stepX = (startPos->x < endPos->x) ? hMovement[DIR_EAST] : hMovement[DIR_WEST];
    gint stepY = (startPos->y < endPos->y) ? vMovement[DIR_SOUTH] : vMovement[DIR_NORTH];
    gint err = changeX + changeY;
    gint e2; // error value e_xy

    e2 = 2 * err;

    // Move along x-axis
    if (e2 >= changeY)
    {
        err += changeY;
        startPos->x += stepX;
    } // e_xy + e_x > 0

    // Move along y-axis
    if (e2 <= changeX)
    {
        err += changeX;
        startPos->y += stepY;
    } // e_xy + e_y < 0

    return startPos;
}

//-------------------------------------------------------------------------------------------------
// Returns the distance between two given points.
guint GetDistanceBetween(Point *point1, Point *point2)
{
    gint changeX = point2->x - point1->x;
    gint changeY = point2->y - point1->y;
    double distance = sqrt( (changeX * changeX) + (changeY * changeY));

    return (guint)distance;
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
    if (position->x >= origin->x && position->x < origin->x + width
      && position->y >= origin->y && position->y < origin->y + height)
        return TRUE;
    else
        return FALSE;
}

// ------------------------------------------------------------------------------------------------
// Allow gtk_main to iterate once so pending screen redraws can be completed.
void WaitForScreenRedraw(void)
{
    while (gtk_events_pending())
    {
        gtk_main_iteration();
    }
}
