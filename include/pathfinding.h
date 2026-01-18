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

enum PathMapStatus
{
    UPDATE_NOT_NEEDED,
    UPDATE_NEEDED,
    UPDATE_STATUS_COUNT
};

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
extern gboolean pathMapNeedsUpdate;

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

Point* GetPathMapOrigin(void);
void SetPathMapOrigin(Point *position);
guint GetPathMapDir(Point *position);
guint GetPathMapDist(Point *position);
gboolean GetPathMapUpdateStatus(void);
gboolean DoesPathToCellExist(Point *position);
void SetPathMapUpdateStatus(PathMapStatus status);
void BuildPathMap(void);

#endif // PATHFINDING_H
