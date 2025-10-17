#include <gtk-2.0/gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <glib-2.0/glib.h>
#include <cstdlib>
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
gboolean IsOutsideDungeon(gint positionX, gint positionY)
{
    if (positionX < 0 || positionX >= DUNGEON_WIDTH)
        return TRUE;
    if (positionY < 0 || positionY >= DUNGEON_HEIGHT)
        return TRUE;

    return FALSE;
}

// ------------------------------------------------------------------------------------------------
// Gets the dungeonCell at the given position.
DungeonCell* GetCellAtPosition(gint positionX, gint positionY)
{
    if (!IsOutsideDungeon(positionX, positionY))
        return &cells[positionY][positionX];
    else
        return NULL;
}

// ------------------------------------------------------------------------------------------------
// Gets the terrain of the dungeonCell at the given position.
Terrain GetCellTerrain(gint positionX, gint positionY)
{
    if (!IsOutsideDungeon(positionX, positionY))
        return cells[positionY][positionX].terrain;
    else
        return TERRAIN_NULL;
}

// ------------------------------------------------------------------------------------------------
// Sets the terrain of the dungeonCell at the given position.
void SetCellTerrain(gint positionX, gint positionY, Terrain terrain)
{
    if (!IsOutsideDungeon(positionX, positionY))
        cells[positionY][positionX].terrain = terrain;
}

// ------------------------------------------------------------------------------------------------
// Gets the pointer of the actor at the given dungeonCell position.
Actor* GetCellsActor(gint positionX, gint positionY)
{
    if (!IsOutsideDungeon(positionX, positionY))
        return cells[positionY][positionX].actor;
    else
        return NULL;
}

// ------------------------------------------------------------------------------------------------
// Sets the pointer for the actor at the given dungeonCell position.
void SetCellsActor(gint positionX, gint positionY, Actor *actor)
{
    if (!IsOutsideDungeon(positionX, positionY))
        cells[positionY][positionX].actor = actor;
}
