#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include <cstdlib>
#include "global.h"
#include "actor.h"
#include "action.h"
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

Action playerActions[MAX_QUEUED_PLAYER_ACTIONS] = {ACTION_NULL};

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

static gboolean ActionWalk(Actor *actor, Direction direction);


// ------------------------------------------------------------------------------------------------
// Gets the player action at the given index in the queue.
Action GetQueuedPlayerAction(guint queueIndex)
{
    return playerActions[queueIndex];
}

// ------------------------------------------------------------------------------------------------
// Sets the player action at the given index in the queue.
void SetQueuedPlayerAction(guint queueIndex, Action action)
{
    playerActions[queueIndex] = action;
}

// ------------------------------------------------------------------------------------------------
// Sets all queued player actions to ACTION_NULL.
void ClearQueuedPlayerActions(void)
{
    for (guint i = 0; i < MAX_QUEUED_PLAYER_ACTIONS; i++)
    {
        playerActions[i] = ACTION_NULL;
    }
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

    return TRUE;
}
