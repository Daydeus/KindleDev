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

TERRAIN dungeonCells[DUNGEON_WIDTH * DUNGEON_HEIGHT] = {TERRAIN_WALL};

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Gets the terrain of the dungeonCell at the given position.
TERRAIN GetCellTerrain(guint positionX, guint positionY)
{
    guint index = (positionY * DUNGEON_WIDTH) + positionX;
    return dungeonCells[index];
}

// ------------------------------------------------------------------------------------------------
// Sets the terrain of the dungeonCell at the given position.
void SetCellTerrain(guint positionX, guint positionY, TERRAIN terrain)
{
    guint index = (positionY * DUNGEON_WIDTH) + positionX;
    dungeonCells[index] = terrain;
}

// ------------------------------------------------------------------------------------------------
// Randomizes the terrain of the dungeonCells.
void RandomizeDungeon(void)
{
    for (guint y = 0; y < DUNGEON_HEIGHT; y++)
    {
        for (guint x = 0; x < DUNGEON_WIDTH; x++)
        {
            guint index = (y * DUNGEON_WIDTH) + x;
            SetCellTerrain(x, y, (TERRAIN)(rand() % 2));
        }
    }
}

