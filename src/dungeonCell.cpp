#include <gtk-2.0/gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
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
// Returns whether the given cell is occupied by an actor.
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
