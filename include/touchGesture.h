#ifndef TOUCH_GESTURE_H
#define TOUCH_GESTURE_H

#include <time.h>
#include "global.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------

enum GestureType
{
    GESTURE_NONE,
    GESTURE_SINGLE_TAP,
    //GESTURE_DOUBLE_TAP, To be implemented later.
    //GESTURE_LONG_PRESS, To be implemented later.
    GESTURE_SWIPE,
    GESTURE_COUNT
};

typedef struct
{
    Point startPos;
    Point endPos;
    time_t startTime;
    time_t endTime;
} TouchGesture;

// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

extern TouchGesture gestureData;

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

Point GetGestureStartPos(void);
void SetGestureStartPos(Point *position);
Point GetGestureEndPos(void);
void SetGestureEndPos(Point *position);
time_t GetGestureStartTime(void);
void SetGestureStartTime(void);
time_t GetGestureEndTime(void);
void SetGestureEndTime(void);
GestureType GetGestureType(void);
Direction GetSwipeDirection(void);

#endif // TOUCH_GESTURE_H
