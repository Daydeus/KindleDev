#ifndef DUNGEON_CELL_H
#define DUNGEON_CELL_H

#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include "actor.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------

#define DUNGEON_WIDTH  60
#define DUNGEON_HEIGHT 40

// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------

enum Terrain
{
    TERRAIN_NULL,
    TERRAIN_FLOOR,
    TERRAIN_WALL,
    TERRAIN_COUNT
};

typedef struct
{
    Terrain terrain;
    Actor *actor;
} DungeonCell;


// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

extern DungeonCell cells[DUNGEON_HEIGHT][DUNGEON_WIDTH];

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

gboolean IsOutsideDungeon(gint positionX, gint positionY);
DungeonCell* GetCellAtPosition(gint positionX, gint positionY);
Terrain GetCellTerrain(gint positionX, gint positionY);
void SetCellTerrain(gint positionX, gint positionY, Terrain terrain);
Actor* GetCellsActor(gint positionX, gint positionY);
void SetCellsActor(gint positionX, gint positionY, Actor *actor);
gboolean IsTerrainTraversable(gint positionX, gint positionY);

#endif // DUNGEON_CELL_H
