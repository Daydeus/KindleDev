#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include <cstdlib>
#include "global.h"
#include "actor.h"
#include "action.h"
#include "dungeonCell.h"
#include "pathfinding.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

gboolean inputIsBlocked = FALSE;

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

static gboolean ActionWalk(Actor *actor, Direction direction);

// ------------------------------------------------------------------------------------------------
// Gets whether the player input is currently blocked.
gboolean GetInputBlockStatus(void)
{
    return inputIsBlocked;
}

// ------------------------------------------------------------------------------------------------
// Sets whether the player input is currently blocked.
void SetInputBlockStatus(gboolean isBlocked)
{
    inputIsBlocked = isBlocked;
}

// ------------------------------------------------------------------------------------------------
// Gets the non-player actor's action.
Action GetActionForAI(void)
{
    gint direction = rand() % DIR_CARDINAL_COUNT + 1;

    return GetWalkFromDirection((Direction)direction);
}

// ------------------------------------------------------------------------------------------------
// Looks up and calls the function for the given action.
// Returns FALSE if the action fails.
gboolean DoAction(Actor *actor, Action action)
{
    gboolean actionCompleted = FALSE;

    switch (action)
    {
    case ACTION_WALK_NORTH:
        actionCompleted = ActionWalk(actor, DIR_NORTH);
        break;
    case ACTION_WALK_EAST:
        actionCompleted = ActionWalk(actor, DIR_EAST);
        break;
    case ACTION_WALK_SOUTH:
        actionCompleted = ActionWalk(actor, DIR_SOUTH);
        break;
    case ACTION_WALK_WEST:
        actionCompleted = ActionWalk(actor, DIR_WEST);
        break;
    case ACTION_WALK_NORTH_EAST:
        actionCompleted = ActionWalk(actor, DIR_NORTH_EAST);
        break;
    case ACTION_WALK_SOUTH_EAST:
        actionCompleted = ActionWalk(actor, DIR_SOUTH_EAST);
        break;
    case ACTION_WALK_SOUTH_WEST:
        actionCompleted = ActionWalk(actor, DIR_SOUTH_WEST);
        break;
    case ACTION_WALK_NORTH_WEST:
        actionCompleted = ActionWalk(actor, DIR_NORTH_WEST);
        break;
    default:
        actionCompleted = FALSE;
    }

    return actionCompleted;
}

// ------------------------------------------------------------------------------------------------
// Attempts to move the given actor one space in the given direction.
// Returns FALSE if the action fails.
static gboolean ActionWalk(Actor *actor, Direction direction)
{
    Point *oldPosition = GetActorPosition(actor);
    Point newPosition;

    newPosition.x = oldPosition->x + hMovement[direction];
    newPosition.y = oldPosition->y + vMovement[direction];

    if (IsOutsideDungeon(&newPosition))
    {
        return FALSE;
    }
    if (!IsTerrainTraversable(&newPosition))
    {
        return FALSE;
    }
    if (IsCellOccupiedByActor(&newPosition))
    {
        return FALSE;
    }

    SetCellsActor(oldPosition, NULL);

    SetCellsActor(&newPosition, actor);
    SetActorPosition(actor, &newPosition);

    // If actor was the player, flag the pathMap as needing an update.
    if (actor == GetActor(PLAYER_ACTOR_INDEX))
        SetPathMapUpdateStatus(UPDATE_NEEDED);

    return TRUE;
}

// ------------------------------------------------------------------------------------------------
// Gets the action for the player to navigate to the selected cell.
Action GetPlayerNavigation(void)
{
    Point *selectedCell = GetSelectedCell();
    Point pathStep = {selectedCell->x, selectedCell->y};
    gint direction = GetPathMapDir(selectedCell);
    gint distance = GetPathMapDist(selectedCell);

    // If we are one move from the selected cell, any walk action will put the player at the
    // destination. So, unlock the selected cell so we don't attempt to navigate again.
    if (distance == 1)
        SetSelectedCellStatus(STATUS_UNLOCKED);

    // Get the next step in the path to the selected cell.
    for (gint i = distance; i > 0; i--)
    {
        direction = GetPathMapDir(&pathStep);

        pathStep.x += hMovement[direction];
        pathStep.y += vMovement[direction];
    }

    // Reverse the direction to move from the path origin towards the selectedCell.
    direction = GetOppositeDirection((Direction)direction);

    return GetWalkFromDirection((Direction)direction);
}

// ------------------------------------------------------------------------------------------------
// Gets the Walk action associated with a given direction.
Action GetWalkFromDirection(Direction direction)
{
    switch (direction)
    {
    case DIR_NORTH:
        return ACTION_WALK_NORTH;
    case DIR_EAST:
        return ACTION_WALK_EAST;
    case DIR_SOUTH:
        return ACTION_WALK_SOUTH;
    case DIR_WEST:
        return ACTION_WALK_WEST;
    case DIR_NORTH_EAST:
        return ACTION_WALK_NORTH_EAST;
    case DIR_SOUTH_EAST:
        return ACTION_WALK_SOUTH_EAST;
    case DIR_SOUTH_WEST:
        return ACTION_WALK_SOUTH_WEST;
    case DIR_NORTH_WEST:
        return ACTION_WALK_NORTH_WEST;
    default:
        return ACTION_NULL;
    }
}
