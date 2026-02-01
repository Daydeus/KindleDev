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

static void InitDungeon(void);
static void PlaceRoom(Room *room);
static void CreateCorridor(Point *startPos, Point *endPos);
static void PlaceEdgeTerrain(void);
static void PlaceStairs(void);
static void PrintTerrainMap(void);

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
            SetCellSightId(&position, CELL_UNEXPLORED);
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
static void CreateCorridor(Point *startPos, Point *endPos)
{
    for (gint y = startPos->y; y != endPos->y; y += (endPos->y > startPos->y ? 1 : -1))
    {
        Point position = {startPos->x, y};

        if (!IsOutsideDungeon(&position))
            SetCellTerrain(&position, TERRAIN_FLOOR);
    }

    for (gint x = startPos->x; x != endPos->x; x += (endPos->x > startPos->x ? 1 : -1))
    {
        Point position = {x, endPos->y};

        if (!IsOutsideDungeon(&position))
            SetCellTerrain(&position, TERRAIN_FLOOR);
    }
}

// ------------------------------------------------------------------------------------------------
// Set the terrain of the dungeon boundaries to TERRAIN_EDGE.
static void PlaceEdgeTerrain(void)
{
    // Set terrain of left and right edges.
    for (gint y = 0; y < DUNGEON_HEIGHT; y++)
    {
        Point edgeCellLeft = {0, y};
        Point edgeCellRight = {DUNGEON_WIDTH - 1, y};

        SetCellTerrain(&edgeCellLeft, TERRAIN_EDGE);
        SetCellTerrain(&edgeCellRight, TERRAIN_EDGE);
    }

    // Set terrain of top and bottom edges.
    for (gint x = 0; x < DUNGEON_WIDTH; x++)
    {
        Point edgeCellTop = {x, 0};
        Point edgeCellBottom = {x, DUNGEON_HEIGHT - 1};

        SetCellTerrain(&edgeCellTop, TERRAIN_EDGE);
        SetCellTerrain(&edgeCellBottom, TERRAIN_EDGE);
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
        rooms[i].start.x = 1 + rand() % (DUNGEON_WIDTH - rooms[i].width);
        rooms[i].start.y = 1 + rand() % (DUNGEON_HEIGHT - rooms[i].height);

        PlaceRoom(&rooms[i]);
        roomCount++;
    }

    // Connect rooms with corridors.
    for (guint i = 0; i < roomCount - 1; i++)
    {
        // Navigate from the center of one room to another.
        Point startPos, endPos;

        startPos.x = rooms[i].start.x + (rooms[i].width / 2);
        startPos.y = rooms[i].start.y + (rooms[i].height / 2);
        endPos.x = rooms[i + 1].start.x + (rooms[i + 1].width / 2);
        endPos.y = rooms[i + 1].start.y + (rooms[i + 1].height / 2);

        CreateCorridor(&startPos, &endPos);
    }

    PlaceEdgeTerrain();
    PlaceStairs();
}

// ------------------------------------------------------------------------------------------------
// Prints the terrain of all cells in the dungeon.
static void PrintTerrainMap(void)
{
    g_print("\n");

    for (gint y = 0; y < DUNGEON_HEIGHT; y++)
    {
        for (gint x = 0; x < DUNGEON_WIDTH; x++)
        {
            Point cell = {x, y};

            switch (GetCellTerrain(&cell))
            {
            case TERRAIN_EDGE:
                g_print("\u2610 ");
                break;
            case TERRAIN_FLOOR:
                g_print(". ");
                break;
            case TERRAIN_STAIRS:
                g_print("^ ");
                break;
            case TERRAIN_WALL:
                g_print("# ");
                break;
            default:
                g_print("? ");
                break;
            }
        }

        g_print("\n");
    }
}
