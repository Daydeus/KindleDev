#include <glib-2.0/glib.h>
#include <cstdlib>
#include "dungeonCell.h"
#include "dungeonGeneration.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------

#define MAX_ROOMS       9
#define ROOM_HEIGHT_MIN 4
#define ROOM_WIDTH_MIN  5

// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------

typedef struct
{
    Point start;
    guint width;
    guint height;
} Room;

// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Set the terrain of all dungeon cells to TERRAIN_WALL.
static void InitDungeon(void)
{
    for (guint y = 0; y < DUNGEON_HEIGHT; y++)
    {
        for (guint x = 0; x < DUNGEON_WIDTH; x++)
        {
            SetCellTerrain(x, y, TERRAIN_WALL);
        }
    }
}

// ------------------------------------------------------------------------------------------------
// Place a rectangle of TERRAIN_FLOOR in the dungeon based on the given room's dimensions.
static void PlaceRoom(Room *room)
{
    for (guint y = room->start.y; y < room->start.y + room->height; y++)
    {
        for (guint x = room->start.x; x < room->start.x + room->width; x++)
        {
            if (!IsOutsideDungeon(x, y))
                SetCellTerrain(x, y, TERRAIN_FLOOR);
        }
    }
}

// ------------------------------------------------------------------------------------------------
// Move from the start to the end position and change each dungeon cell's terrain to TERRAIN_FLOOR.
static void CreateCorridor(guint startX, guint startY, guint endX, guint endY)
{
    for (guint y = startY; y != endY; y += (endY > startY ? 1 : -1))
    {
        if (!IsOutsideDungeon(startX, y))
            SetCellTerrain(startX, y, TERRAIN_FLOOR);
    }

    for (guint x = startX; x != endX; x += (endX > startX ? 1 : -1))
    {
        if (!IsOutsideDungeon(x, endY))
            SetCellTerrain(x, endY, TERRAIN_FLOOR);
    }
}

// ------------------------------------------------------------------------------------------------
// Replace all dungeon cell's terrain with TERRAIN_WALL and add rooms and corridors of TERRAIN_FLOOR.
void GenerateDungeon(void)
{
    Room rooms[MAX_ROOMS];
    guint roomCount = 0;

    // Ensure that generation starts with a blank slate of TERRAIN_WALL.
    InitDungeon();

    // Generate and place rooms.
    for (guint i = 0; i < MAX_ROOMS; i++)
    {
        rooms[i].width = ROOM_WIDTH_MIN + rand() % 8;
        rooms[i].height = ROOM_HEIGHT_MIN + rand() % 8;
        rooms[i].start.x = rand() % (DUNGEON_WIDTH - rooms[i].width);
        rooms[i].start.y = rand() % (DUNGEON_HEIGHT - rooms[i].height);

        PlaceRoom(&rooms[i]);
        roomCount++;
    }

    // Connect rooms with corridors.
    for (guint i = 0; i < roomCount - 1; i++)
    {
        // Navigate from the center of one room to another.
        guint startX = rooms[i].start.x + (rooms[i].width / 2);
        guint startY = rooms[i].start.y + (rooms[i].height / 2);
        guint endX = rooms[i + 1].start.x + (rooms[i + 1].width / 2);
        guint endY = rooms[i + 1].start.y + (rooms[i + 1].height / 2);

        CreateCorridor(startX, startY, endX, endY);
    }
}
