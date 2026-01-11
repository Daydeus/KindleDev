#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <glib-2.0/glib.h>
#include "global.h"
#include "dungeonCell.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------

struct Path
{
    Direction direction;
    guint distance;
};

// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

extern Point pathMapOrigin;
extern Path pathMap[DUNGEON_HEIGHT][DUNGEON_WIDTH];

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

Point* GetPathMapOrigin(void);
void SetPathMapOrigin(Point *position);
guint GetPathMapDir(Point *position);
guint GetPathMapDist(Point *position);
void BuildPathMap(void);

#endif // PATHFINDING_H
