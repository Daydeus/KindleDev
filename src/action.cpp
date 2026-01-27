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

#define FLEE_DISTANCE_MIN 3

// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

Action playerAction =  ACTION_NONE;
gboolean inputIsBlocked = FALSE;

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

static gboolean ActionWalk(Actor *actor, Direction direction);
static gboolean ActionWalkAuto(void);
static gboolean ActionTerrainFlip(Actor *actor, Point *target);

// ------------------------------------------------------------------------------------------------
// Returns the next action to be performed by the player.
Action GetActionForPlayer(void)
{
    return playerAction;
}

// ------------------------------------------------------------------------------------------------
// Sets the next action to be performed by the player.
void SetActionForPlayer(Action action)
{
    playerAction = action;
}

// ------------------------------------------------------------------------------------------------
// Gets the non-player actor's action.
Action GetActionForAI(Actor *actor)
{
    Point *actorPos = &actor->position;
    guint currentDistanceFromPlayer = GetPathMapDist(actorPos);

    // Flee from player if distance is below the minimum desired.
    if (currentDistanceFromPlayer < FLEE_DISTANCE_MIN)
    {
        Direction directionToMove = DIR_NONE;
        guint maxDistanceFromPlayer = 0;

        for (guint i = DIR_NORTH; i < DIR_ALL_COUNT; i++)
        {
            Point neighborCell = *actorPos;
            guint neighborDistance = 0;

            neighborCell.x += hMovement[i];
            neighborCell.y += vMovement[i];

            neighborDistance = GetPathMapDist(&neighborCell);

            if (neighborDistance > maxDistanceFromPlayer && IsTerrainTraversable(&neighborCell))
            {
                maxDistanceFromPlayer = neighborDistance;
                directionToMove = (Direction)i;
            }
        }

        return GetWalkFromDirection(directionToMove);
    }
    else
    {
        guint attemptCount = 0;

        // Pick a a random direction to move in. Attempt up to ten times before skipping turn.
        while (attemptCount < 10)
        {
            Point neighborCell = *actorPos;
            guint directionToMove = rand() % DIR_ALL_COUNT;

            neighborCell.x += hMovement[directionToMove];
            neighborCell.y += vMovement[directionToMove];

            if (GetPathMapDist(&neighborCell) > FLEE_DISTANCE_MIN && IsTerrainTraversable(&neighborCell))
                return GetWalkFromDirection((Direction)directionToMove);

            attemptCount++;
        }

        return ACTION_NONE;
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
    case ACTION_WALK_AUTO:
        actionCompleted = ActionWalkAuto();
        break;
    case ACTION_TERRAIN_FLIP:
        actionCompleted = ActionTerrainFlip(actor, GetSelectedCell());
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
// Attempts to ACTION_WALK the player in the direction of the next cell on the path to the
// selectedCell. Returns FALSE if the action fails.
gboolean ActionWalkAuto(void)
{
    Point *selectedCell = GetSelectedCell();
    Point pathStep = {selectedCell->x, selectedCell->y};
    gint direction = GetPathMapDir(selectedCell);
    gint distance = GetPathMapDist(selectedCell);

    // The selectedCell cannot be navigated to, so abort action.
    if (!DoesPathToCellExist(selectedCell))
        return ACTION_NONE;

    // Completion of the current action will place the player at the selectedCell, so unlock the
    // cell selector icon.
    if (distance == 1)
        SetSelectedCellStatus(STATUS_OFF);

    // Get the next step in the path to the selected cell.
    for (gint i = distance; i > 0; i--)
    {
        direction = GetPathMapDir(&pathStep);

        pathStep.x += hMovement[direction];
        pathStep.y += vMovement[direction];
    }

    // Reverse the direction to move from the path origin towards the selectedCell.
    direction = GetOppositeDirection((Direction)direction);

    return ActionWalk(GetActor(PLAYER_ACTOR_INDEX), (Direction)direction);
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
        return ACTION_NONE;
    }
}

// ------------------------------------------------------------------------------------------------
// Attempts to change the terrain at the given actor's target position.
// Returns FALSE if the action fails.
static gboolean ActionTerrainFlip(Actor *actor, Point *target)
{
    Terrain terrain = GetCellTerrain(target);

    if (IsOutsideDungeon(target))
    {
        return FALSE;
    }
    if (IsCellOccupiedByActor(target))
    {
        return FALSE;
    }
    // TODO: Add fail condition if the target is not in the actor's Line-Of-Sight.

    if (terrain == TERRAIN_WALL)
        SetCellTerrain(target, TERRAIN_FLOOR);
    else if (terrain == TERRAIN_FLOOR)
        SetCellTerrain(target, TERRAIN_WALL);

    // If terrain was changed, the pathMap must be updated.
    SetPathMapUpdateStatus(UPDATE_NEEDED);

    return TRUE;
}
