#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include <cstdlib>
#include "global.h"
#include "dungeonCell.h"
#include "actor.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

DungeonCell cells[DUNGEON_HEIGHT][DUNGEON_WIDTH] = {};
Point selectedCell = {0};
CellSelectorStatus selectedCellLocked = STATUS_OFF;

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Returns true if the given position is not within the dungeon.
gboolean IsOutsideDungeon(Point *position)
{
    Point mapOrigin = {0, 0};

    if (IsWithinRectangle(position, &mapOrigin, DUNGEON_WIDTH, DUNGEON_HEIGHT))
        return FALSE;
    else
        return TRUE;
}

// ------------------------------------------------------------------------------------------------
// Gets the dungeonCell at the given position.
DungeonCell* GetCellAtPosition(Point *position)
{
    if (!IsOutsideDungeon(position))
        return &cells[position->y][position->x];
    else
        return NULL;
}

// ------------------------------------------------------------------------------------------------
// Gets the terrain of the dungeonCell at the given position.
Terrain GetCellTerrain(Point *position)
{
    if (!IsOutsideDungeon(position))
        return cells[position->y][position->x].terrain;
    else
        return TERRAIN_NULL;
}

// ------------------------------------------------------------------------------------------------
// Sets the terrain of the dungeonCell at the given position.
void SetCellTerrain(Point *position, Terrain terrain)
{
    if (!IsOutsideDungeon(position))
        cells[position->y][position->x].terrain = terrain;
}

// ------------------------------------------------------------------------------------------------
// Gets the pointer of the actor at the given dungeonCell position.
Actor* GetCellsActor(Point *position)
{
    if (!IsOutsideDungeon(position))
        return cells[position->y][position->x].actor;
    else
        return NULL;
}

// ------------------------------------------------------------------------------------------------
// Sets the pointer for the actor at the given dungeonCell position.
void SetCellsActor(Point *position, Actor *actor)
{
    if (!IsOutsideDungeon(position))
        cells[position->y][position->x].actor = actor;
}

// ------------------------------------------------------------------------------------------------
// Returns whether the given cell's terrain is traversable.
gboolean IsTerrainTraversable(Point *position)
{
    if (IsOutsideDungeon(position))
    {
        return FALSE;
    }
    else
    {
        Terrain terrain = GetCellTerrain(position);

        if (terrain == TERRAIN_FLOOR || terrain == TERRAIN_STAIRS)
            return TRUE;
        else
            return FALSE;
    }
}

// ------------------------------------------------------------------------------------------------
// Returns TRUE if the given cell is occupied by an actor (or if the position is outside the dungeon).
gboolean IsCellOccupiedByActor(Point *position)
{
    if (IsOutsideDungeon(position))
    {
        return TRUE;
    }
    else
    {
        Actor *actor = GetCellsActor(position);

        // Check if cell already contains an actor.
        if (actor != NULL)
            return TRUE;
        else
            return FALSE;
    }
}

// ------------------------------------------------------------------------------------------------
// Returns TRUE if the given diagonal movement direction to the given cell would be blocked due to
// non-traversable neighboring cells.
gboolean IsCellBlockedDiagonally(Point *position, Direction direction)
{
    if (IsOutsideDungeon(position))
    {
        return TRUE;
    }
    else
    {
        Direction neighborDir1, neighborDir2 = DIR_NONE;

        switch (direction)
        {
        case DIR_NORTH_EAST:
            neighborDir1 = DIR_SOUTH;
            neighborDir2 = DIR_WEST;
            break;
        case DIR_SOUTH_EAST:
            neighborDir1 = DIR_NORTH;
            neighborDir2 = DIR_WEST;
            break;
        case DIR_SOUTH_WEST:
            neighborDir1 = DIR_NORTH;
            neighborDir2 = DIR_EAST;
            break;
        case DIR_NORTH_WEST:
            neighborDir1 = DIR_EAST;
            neighborDir2 = DIR_SOUTH;
            break;
        default:
            return FALSE;
        }

        Point neighbor1 = {position->x + hMovement[neighborDir1], position->y + vMovement[neighborDir1]};
        Point neighbor2 = {position->x + hMovement[neighborDir2], position->y + vMovement[neighborDir2]};

        if (IsTerrainTraversable(&neighbor1) || IsTerrainTraversable(&neighbor2))
            return FALSE;
    }

    return TRUE;
}

// ------------------------------------------------------------------------------------------------
// Gets the dungeonCell position of the currently selected cell in the viewPort.
Point* GetSelectedCell(void)
{
    return &selectedCell;
}

// ------------------------------------------------------------------------------------------------
// Sets the currently selected dungeonCell in the viewPort to the given position.
void SetSelectedCell(Point *position)
{
    position->x = ClampValue(position->x, 0, DUNGEON_WIDTH);
    position->y = ClampValue(position->y, 0, DUNGEON_HEIGHT);

    selectedCell = *position;
}

// ------------------------------------------------------------------------------------------------
// Gets whether input is locked for moving to the selected cell.
CellSelectorStatus GetSelectedCellStatus(void)
{
    return selectedCellLocked;
}

// ------------------------------------------------------------------------------------------------
// Sets whether input is locked for moving to the selected cell.
void SetSelectedCellStatus(CellSelectorStatus status)
{
    selectedCellLocked = status;
}
