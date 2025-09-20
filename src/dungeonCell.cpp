#include <gtk-2.0/gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <glib-2.0/glib.h>
#include <cstdlib>
#include "dungeonCell.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

TERRAIN dungeonCells[DUNGEON_WIDTH * DUNGEON_HEIGHT] = {TERRAIN_NULL};

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
// Gets the terrain of the dungeonCell at the given position.
TERRAIN GetCellTerrain(guint positionX, guint positionY)
{
    guint index = (positionY * DUNGEON_WIDTH) + positionX;

    if (!IsOutsideDungeon(positionX, positionY))
        return dungeonCells[index];
    else
        return TERRAIN_NULL;
}

// ------------------------------------------------------------------------------------------------
// Sets the terrain of the dungeonCell at the given position.
void SetCellTerrain(guint positionX, guint positionY, TERRAIN terrain)
{
    guint index = (positionY * DUNGEON_WIDTH) + positionX;
    dungeonCells[index] = terrain;
}
