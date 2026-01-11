#include <cstdlib>
#include <glib-2.0/glib.h>
#include "global.h"
#include "pathfinding.h"
#include "dungeonCell.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------

#define MAX_DISTANCE 999

// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------

struct Node
{
    Point position;
    Direction direction;
    guint distance;
    struct Node* nextNode;
};

// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

Point pathMapOrigin = {0};
Path pathMap[DUNGEON_HEIGHT][DUNGEON_WIDTH];

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

static void SetPathMapDir(Point *position, Direction direction);
static void SetPathMapDist(Point *position, guint distance);
static Node* AddFrontierNode(Node* listHead, Point *position, Direction direction, guint distance);
static Node* DeleteFrontierNode(Node *listHead);
static guint GetListNodeCount(Node *listHead);
static void PrintDirectionMap(void);
static void PrintDistanceMap(void);

// ------------------------------------------------------------------------------------------------
// Returns the position of the cell the pathMap paths to.
Point* GetPathMapOrigin(void)
{
    return &pathMapOrigin;
}

// ------------------------------------------------------------------------------------------------
// Sets the position the pathMap paths to when building the map.
void SetPathMapOrigin(Point *position)
{
    position->x = ClampValue(position->x, 0, DUNGEON_WIDTH);
    position->y = ClampValue(position->y, 0, DUNGEON_HEIGHT);

    pathMapOrigin = *position;
}

// ------------------------------------------------------------------------------------------------
// Returns the direction to the adjacent cell that is closest to the pathMapOrigin.
guint GetPathMapDir(Point *position)
{
    return pathMap[position->y][position->x].direction;
}

// ------------------------------------------------------------------------------------------------
// Sets the direction to the adjacent cell that is closest to the pathMapOrigin.
static void SetPathMapDir(Point *position, Direction direction)
{
    pathMap[position->y][position->x].direction = direction;
}

// ------------------------------------------------------------------------------------------------
// Gets the distance from the pathMapOrigin.
guint GetPathMapDist(Point *position)
{
    return pathMap[position->y][position->x].distance;
}

// ------------------------------------------------------------------------------------------------
// Sets the distance from the pathMapOrigin.
static void SetPathMapDist(Point *position, guint distance)
{
    pathMap[position->y][position->x].distance = distance;
}

// ------------------------------------------------------------------------------------------------
// Builds a map that stores the direction and distance from all cells to a single origin cell.
void BuildPathMap(void)
{
    Node *listHead = NULL;
    Point *originCell = GetPathMapOrigin();

    // Initialize pathMap for all cells.
    for (gint y = 0; y < DUNGEON_HEIGHT; y++)
    {
        for (gint x = 0; x < DUNGEON_WIDTH; x++)
        {
            Point currentCell = {x, y};

            SetPathMapDir(&currentCell, DIR_NONE);
            SetPathMapDist(&currentCell, MAX_DISTANCE);
        }
    }

    // Add the pathMapOrigin cell to the frontier linked list.
    listHead = AddFrontierNode(listHead, originCell, DIR_NONE, 0);

    // Initalize pathMap values for the origin cell.
    SetPathMapDir(originCell, DIR_NONE);
    SetPathMapDist(originCell, 0);

    // Loop until all cells have been explored.
    while (listHead != NULL)
    {
        Point currentCell = listHead->position;

        // Add adjacent cells in all directions to the frontier list.
        for (gint dir = DIR_NORTH; dir < DIR_ALL_COUNT; dir++)
        {
            Point neighborCell = {currentCell.x + hMovement[dir], currentCell.y + vMovement[dir]};

            if (!IsOutsideDungeon(&neighborCell) && IsTerrainTraversable(&neighborCell)
                && pathMap[neighborCell.y][neighborCell.x].distance == MAX_DISTANCE)
            {
                Direction travelDir = GetOppositeDirection((Direction)dir);
                guint travelDist = listHead->distance + 1;

                listHead = AddFrontierNode(listHead, &neighborCell, travelDir, travelDist);

                SetPathMapDir(&neighborCell, travelDir);
                SetPathMapDist(&neighborCell, travelDist);
            }
        }

        // Remove currentCell now that it has been explored.
        listHead = DeleteFrontierNode(listHead);
    }
}

// ------------------------------------------------------------------------------------------------
// Add a new node to the end of the frontier linked list.
static Node* AddFrontierNode(Node* listHead, Point *position, Direction direction, guint distance)
{
    // Allocate memory for the new node.
    Node* newNode = (Node*)malloc(sizeof(Node));

    newNode->position = *position;
    newNode->direction = direction;
    newNode->distance = distance;
    newNode->nextNode = NULL;

    // If no list exists yet.
    if (listHead == NULL)
    {
        listHead = newNode;
    }
    else // Add newNode to end of list.
    {
        Node* currentNode = listHead;

        // Find the last node in the list.
        while (currentNode->nextNode != NULL)
        {
            currentNode = currentNode->nextNode;
        }

        currentNode->nextNode = newNode;
    }

    return listHead;
}

// ------------------------------------------------------------------------------------------------
// Remove the listHead and make the next node in the list the head, if there is one.
static Node* DeleteFrontierNode(Node *listHead)
{
    // If listHead is NULL, there is no linked list.
    if (listHead == NULL)
        return listHead;

    Node *newHead = listHead->nextNode;

    free(listHead);
    listHead = NULL;

    return newHead;
}

// ------------------------------------------------------------------------------------------------
// Returns the number of nodes in the Frontier list.
static guint GetListNodeCount(Node *listHead)
{
    guint nodeCount = 0;
    Node *current = listHead;

    while (current != NULL)
    {
        nodeCount++;
        current = current->nextNode;
    }

    return nodeCount;
}

// ------------------------------------------------------------------------------------------------
// Prints the travel direction from all cells to the origin cell of the pathMap.
static void PrintDirectionMap(void)
{
    Point *originCell = GetPathMapOrigin();

    g_print("\n");

    for (gint y = 0; y < DUNGEON_HEIGHT; y++)
    {
        for (gint x = 0; x < DUNGEON_WIDTH; x++)
        {
            if (x == originCell->x  && y == originCell->y)
                g_print("@ ");
            else
                g_print("%s ", GetDirectionSymbol(pathMap[y][x].direction));
        }

        g_print("\n");
    }
}

// ------------------------------------------------------------------------------------------------
// Prints the travel distance from all cells to the origin cell of the pathMap.
static void PrintDistanceMap(void)
{
    Point *originCell = GetPathMapOrigin();

    g_print("\n");

    for (gint y = 0; y < DUNGEON_HEIGHT; y++)
    {
        for (gint x = 0; x < DUNGEON_WIDTH; x++)
        {
            if (x == originCell->x  && y == originCell->y)
                g_print("@ ");
            else if (pathMap[y][x].distance == MAX_DISTANCE)
                g_print(". ");
            else
                g_print("%d ", pathMap[y][x].distance);
        }

        g_print("\n");
    }
}
