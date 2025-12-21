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
    gint width;
    gint height;
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
    for (gint y = 0; y < DUNGEON_HEIGHT; y++)
    {
        for (gint x = 0; x < DUNGEON_WIDTH; x++)
        {
            Point position = {x, y};

            SetCellTerrain(&position, TERRAIN_WALL);
        }
    }
}

// ------------------------------------------------------------------------------------------------
// Place a rectangle of TERRAIN_FLOOR in the dungeon based on the given room's dimensions.
static void PlaceRoom(Room *room)
{
    for (gint y = room->start.y; y < room->start.y + room->height; y++)
    {
        for (gint x = room->start.x; x < room->start.x + room->width; x++)
        {
            Point position = {x, y};

            if (!IsOutsideDungeon(&position))
                SetCellTerrain(&position, TERRAIN_FLOOR);
        }
    }
}

// ------------------------------------------------------------------------------------------------
// Move from the start to the end position and change each dungeon cell's terrain to TERRAIN_FLOOR.
static void CreateCorridor(gint startX, gint startY, gint endX, gint endY)
{
    for (gint y = startY; y != endY; y += (endY > startY ? 1 : -1))
    {
        Point position = {startX, y};

        if (!IsOutsideDungeon(&position))
            SetCellTerrain(&position, TERRAIN_FLOOR);
    }

    for (gint x = startX; x != endX; x += (endX > startX ? 1 : -1))
    {
        Point position = {x, endY};

        if (!IsOutsideDungeon(&position))
            SetCellTerrain(&position, TERRAIN_FLOOR);
    }
}

// ------------------------------------------------------------------------------------------------
// Place a single instance of TERRAIN_STAIRS within the dungeon.
static void PlaceStairs(void)
{
    Point position = {0};

    do
    {
        position = {rand() % DUNGEON_WIDTH, rand() % DUNGEON_HEIGHT};

    } while (GetCellTerrain(&position) != TERRAIN_FLOOR);

    SetCellTerrain(&position, TERRAIN_STAIRS);
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
        gint startX = rooms[i].start.x + (rooms[i].width / 2);
        gint startY = rooms[i].start.y + (rooms[i].height / 2);
        gint endX = rooms[i + 1].start.x + (rooms[i + 1].width / 2);
        gint endY = rooms[i + 1].start.y + (rooms[i + 1].height / 2);

        CreateCorridor(startX, startY, endX, endY);
    }

    PlaceStairs();
}
