#ifndef DUNGEON_CELL_H
#define DUNGEON_CELL_H

#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------

#define DUNGEON_WIDTH  30
#define DUNGEON_HEIGHT 20

// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------

enum TERRAIN
{
    TERRAIN_WALL,
    TERRAIN_FLOOR,
    TERRAIN_COUNT
};

// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

extern TERRAIN dungeonCells[DUNGEON_WIDTH * DUNGEON_HEIGHT];

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

gboolean IsOutsideDungeon(gint positionX, gint positionY);
TERRAIN GetCellTerrain(guint positionX, guint positionY);
void SetCellTerrain(guint positionX, guint positionY, TERRAIN terrain);
void RandomizeDungeon(GtkWidget *widget, gpointer data);

#endif // DUNGEON_CELL_H
