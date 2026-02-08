#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include <cstdlib>
#include "global.h"
#include "dungeonCell.h"
#include "fieldOfView.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

guint playerSightId = 0;
gboolean debug_fogOfWarIsOn = TRUE;

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

static void MarkLineOfSight(Point *source, Point *perimeter);
static void MarkLineOfSightReverse(Point *perimeter, Point *source);

// ------------------------------------------------------------------------------------------------
// Returns the value of playerSightId.
guint GetPlayerSightId(void)
{
    return playerSightId;
}

// ------------------------------------------------------------------------------------------------
// Sets the value of playerSightId.
void SetPlayerSightId(guint newValue)
{
    playerSightId = newValue;
}

// ------------------------------------------------------------------------------------------------
// Sets the sightId of all cells visible to the player equal to the player's current sightId.
void UpdateFOV(Point *center, gint sightRange)
{
    gint x = 0;
    gint y = -sightRange;
    gint param = -sightRange;

    // Loops through each point along the curve of the circle.
    while (x < -y)
    {
        if (param > 0)
        {
            y += 1;
            param += 2 * (x + y) + 1;
        }
        else
        {
            param += 2 * x + 1;
        }

        // Updates the current point in each octant of the circle.
        for (guint octant = DIR_NORTH; octant < DIR_ALL_COUNT; octant++)
        {
            // TODO: Find solution to cells being obscured when they shouldn't be in long
            // corridors.

            Point point = {center->x, center->y};

            switch (octant)
            {
            case DIR_NORTH:
                point.x = center->x + x;
                point.y = center->y + y;
                break;
            case DIR_EAST:
                point.x = center->x - y;
                point.y = center->y + x;
                break;
            case DIR_SOUTH:
                point.x = center->x - x;
                point.y = center->y - y;
                break;
            case DIR_WEST:
                point.x = center->x + y;
                point.y = center->y - x;
                break;
            case DIR_NORTH_EAST:
                point.x = center->x - y;
                point.y = center->y - x;
                break;
            case DIR_SOUTH_EAST:
                point.x = center->x + x;
                point.y = center->y - y;
                break;
            case DIR_SOUTH_WEST:
                point.x = center->x + y;
                point.y = center->y + x;
                break;
            case DIR_NORTH_WEST:
                point.x = center->x - x;
                point.y = center->y + y;
                break;
            }

            MarkLineOfSight(center, &point);
            MarkLineOfSightReverse(&point, center);
        }

        x += 1;
    }
}

// ------------------------------------------------------------------------------------------------
// Loops along a line from the view source to the given point on the field of view perimeter and
// marks as visible all traversable cells.
static void MarkLineOfSight(Point *source, Point *perimeter)
{
    guint escapeCounter = 0;
    Point currentPos = *source;
    Point previousPos = *source;

    while (escapeCounter < 100)
    {
        // Do not allow line of sight to cut diagonally through non-traversable cells.
        if (IsCellBlockedDiagonally(&currentPos, GetTravelDirectionBetweenCells(&previousPos, &currentPos)))
        {
            // Allow wall corners to be made visible, however.
            if (!IsTerrainTraversable(&currentPos))
                SetCellSightId(&currentPos, GetPlayerSightId());
            break;
        }

        SetCellSightId(&currentPos, GetPlayerSightId());

        if (IsSamePoint(&currentPos, perimeter) || !IsTerrainTraversable(&currentPos))
            break;

        previousPos = currentPos;
        currentPos = *GetNextPointOnLine(&currentPos, perimeter);
        escapeCounter++;
    }
}

// ------------------------------------------------------------------------------------------------
// Loops along a line from the given point on the field of view perimeter to the view source and
// marks as visible all traversable cells not blocked from the view source.
static void MarkLineOfSightReverse(Point *perimeter, Point *source)
{
    guint escapeCounter = 0;
    Point newPerimeter = *perimeter; // Farthest non-traversable cell with unblocked line of sight.
    Point currentPos = *perimeter;   // Iterates through whole line to find newPerimeter.
    Point previousPos = *perimeter;  // Used to track if cutting diagonally between cells.

    // Find the non-traversable cell farthest from the view source that has an unblocked line of
    // sight to it and set it as the new perimeter point.
    while (escapeCounter < 100)
    {
        if (!IsTerrainTraversable(&currentPos)
            || IsCellBlockedDiagonally(&currentPos, GetTravelDirectionBetweenCells(&previousPos, &currentPos)))
        {
            newPerimeter = currentPos;
        }

        if (IsSamePoint(&currentPos, source))
            break;

        previousPos = currentPos;
        currentPos = *GetNextPointOnLine(&currentPos, source);
        escapeCounter++;
    }

    // Mark all cells from the new perimeter to the view source as visible.
    while (escapeCounter < 100)
    {
        SetCellSightId(&newPerimeter, GetPlayerSightId());

        if (IsSamePoint(&newPerimeter, source))
            break;

        newPerimeter = *GetNextPointOnLine(&newPerimeter, source);
        escapeCounter++;
    }
}

// ------------------------------------------------------------------------------------------------
// Returns TRUE if the given position is visible to the player.
gboolean IsVisibleToPlayer(Point *position)
{
    if (GetCellSightId(position) == GetPlayerSightId())
        return TRUE;
    else
        return FALSE;
}

// ------------------------------------------------------------------------------------------------
// Prints the current sightID for all cells in the dungeon.
void PrintFOV(void)
{
    g_print("\n");

    for (gint y = 0; y < DUNGEON_HEIGHT; y++)
    {
        for (gint x = 0; x < DUNGEON_WIDTH; x++)
        {
            Point cell = {x, y};

            if (GetCellsActor(&cell) == GetActor(PLAYER_ACTOR_INDEX))
                g_print("@ ");
            else
                g_print("%d ", GetCellSightId(&cell));
        }

        g_print("\n");
    }
}

// ------------------------------------------------------------------------------------------------
// Returns the current status of the debug variable fogOfWarIsOn.
gboolean GetFogOfWarStatus(void)
{
    return debug_fogOfWarIsOn;
}

// ------------------------------------------------------------------------------------------------
// Sets the current status of the debug variable fogOfWarIsOn.
void SetFogOfWarStatus(gboolean status)
{
    debug_fogOfWarIsOn = status;
}
