#include <stdlib.h>
#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include "time.h"
#include "touchGesture.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------

#define MIN_HOLD_TIME   2  // Hold time in seconds for a gesture to be a GESTURE_LONG_PRESS.
#define MIN_SWIPE_DIST  64 // Distance in pixels for a gesture to be a GESTURE_SWIPE.

// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

TouchGesture gestureData = {0};

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Gets the start position of the gesture.
Point GetGestureStartPos(void)
{
    return gestureData.startPos;
}

// ------------------------------------------------------------------------------------------------
// Sets the start position of the gesture.
void SetGestureStartPos(Point *position)
{
    gestureData.startPos.x = position->x;
    gestureData.startPos.y = position->y;
}

// ------------------------------------------------------------------------------------------------
// Gets the end position of the gesture.
Point GetGestureEndPos(void)
{
    return gestureData.endPos;
}

// ------------------------------------------------------------------------------------------------
// Sets the end position of the gesture.
void SetGestureEndPos(Point *position)
{
    gestureData.endPos.x = position->x;
    gestureData.endPos.y = position->y;
}

// ------------------------------------------------------------------------------------------------
// Gets the start time of the gesture.
time_t GetGestureStartTime(void)
{
    return gestureData.startTime;
}

// ------------------------------------------------------------------------------------------------
// Sets the start time of the gesture.
void SetGestureStartTime(void)
{
    gestureData.startTime = time(NULL);
}

// ------------------------------------------------------------------------------------------------
// Gets the end time of the gesture.
time_t GetGestureEndTime(void)
{
    return gestureData.endTime;
}

// ------------------------------------------------------------------------------------------------
// Sets the end time of the gesture.
void SetGestureEndTime(void)
{
    gestureData.endTime = time(NULL);
}

// ------------------------------------------------------------------------------------------------
// Gets the gesture type based on gestureData.
GestureType GetGestureType(void)
{
    Point endPos = GetGestureEndPos();
    Point startPos = GetGestureStartPos();
    gint distanceX = abs(endPos.x - startPos.x);
    gint distanceY = abs(endPos.y - startPos.y);
    double duration = difftime(GetGestureEndTime(), GetGestureStartTime());


    if (distanceX < MIN_SWIPE_DIST && distanceY < MIN_SWIPE_DIST && duration < MIN_HOLD_TIME)
        return GESTURE_SINGLE_TAP;
    else if ((distanceX >= MIN_SWIPE_DIST || distanceY >= MIN_SWIPE_DIST) && duration < MIN_HOLD_TIME)
        return GESTURE_SWIPE;
    else
        return GESTURE_SINGLE_TAP;
}

// ------------------------------------------------------------------------------------------------
// Gets the swipe gesture direction.
Direction GetSwipeDirection(void)
{
    Point endPos = GetGestureEndPos();
    Point startPos = GetGestureStartPos();
    Direction horizontal = DIR_NONE;
    Direction vertical = DIR_NONE;

    // Check if the horizontal distance meets the swipe minimum.
    if (endPos.x > startPos.x + MIN_SWIPE_DIST)
        horizontal = DIR_EAST;
    else if (endPos.x < startPos.x - MIN_SWIPE_DIST)
        horizontal = DIR_WEST;

    // Check if the vertical distance meets the swipe minimum.
    if (endPos.y > startPos.y + MIN_SWIPE_DIST)
        vertical = DIR_SOUTH;
    else if (endPos.y < startPos.y - MIN_SWIPE_DIST)
        vertical = DIR_NORTH;

    if (horizontal == DIR_NONE)
    {
        // Swipe is only up or down.
        return vertical;
    }
    else if (vertical == DIR_NONE)
    {
        // Swipe is only left or right.
        return horizontal;
    }
    else
    {
        // Swipe is diagonal.
        if (vertical == DIR_NORTH && horizontal == DIR_EAST)
            return DIR_NORTH_EAST;
        else if (vertical == DIR_SOUTH && horizontal == DIR_EAST)
            return DIR_SOUTH_EAST;
        else if (vertical == DIR_SOUTH && horizontal == DIR_WEST)
            return DIR_SOUTH_WEST;
        else if (vertical == DIR_NORTH && horizontal == DIR_WEST)
            return DIR_NORTH_WEST;
    }

    return DIR_NONE;
}
