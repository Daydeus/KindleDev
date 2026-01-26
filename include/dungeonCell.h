#ifndef DUNGEON_CELL_H
#define DUNGEON_CELL_H

#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include "global.h"
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
    TERRAIN_STAIRS,
    TERRAIN_WALL,
    TERRAIN_COUNT
};

enum CellSelectorStatus
{
    STATUS_OFF,
    STATUS_UNLOCKED,
    STATUS_LOCKED,
    STATUS_COUNT
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
extern Point selectedCell;
extern CellSelectorStatus selectedCellLocked;

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

gboolean IsOutsideDungeon(Point *position);
DungeonCell* GetCellAtPosition(Point *position);
Terrain GetCellTerrain(Point *position);
void SetCellTerrain(Point *position, Terrain terrain);
Actor* GetCellsActor(Point *position);
void SetCellsActor(Point *position, Actor *actor);
gboolean IsTerrainTraversable(Point *position);
gboolean IsCellOccupiedByActor(Point *position);
gboolean IsCellBlockedDiagonally(Point *position, Direction direction);
Point* GetSelectedCell(void);
void SetSelectedCell(Point *position);
CellSelectorStatus GetSelectedCellStatus(void);
void SetSelectedCellStatus(CellSelectorStatus status);

#endif // DUNGEON_CELL_H
