#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include <cstdlib>
#include "global.h"
#include "actor.h"
#include "action.h"
#include "dungeonCell.h"
#include "dungeonGeneration.h"
#include "fieldOfView.h"
#include "menu.h"
#include "pathfinding.h"
#include "viewPort.h"

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
guint skills[MAX_SKILL_SLOTS] = {SKILL_NONE};

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

static gboolean ActionWalk(Actor *actor, Direction direction);
static gboolean ActionWalkAuto(void);
static gboolean ActionAdvanceFloor(void);
static gboolean ActionAttack(Actor *actor, Direction direction);
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
// Returns the associated action for the given skill.
Action LookupActionForSkill(Skill skill, Direction direction)
{
    switch (skill)
    {
    case SKILL_WALK_TO:
        return ACTION_WALK_AUTO;
    case SKILL_ATTACK_BASIC:
        return GetAttackFromDirection(direction);
    default:
        return ACTION_NONE;
    }
}

// ------------------------------------------------------------------------------------------------
// Gets the non-player actor's action.
Action GetActionForAI(Actor *actor)
{
    Point *actorPos = &actor->position;
    guint currentDistanceFromPlayer = GetPathMapDist(actorPos);

    // Move towards and attack player if actor is within sightRange and has LOS.
    if (currentDistanceFromPlayer <= GetActorSightRange(actor) && IsVisibleToPlayer(actorPos))
    {
        Direction direction = DIR_NONE;
        guint minDistanceToPlayer = GetActorSightRange(actor);

        for (guint i = DIR_NORTH; i < DIR_ALL_COUNT; i++)
        {
            Point neighborCell = *actorPos;
            guint neighborDistance = 0;

            neighborCell.x += hMovement[i];
            neighborCell.y += vMovement[i];

            neighborDistance = GetPathMapDist(&neighborCell);

            if (neighborDistance <= minDistanceToPlayer && IsTerrainTraversable(&neighborCell)
                && !IsCellBlockedDiagonally(&neighborCell, (Direction)i))
            {
                minDistanceToPlayer = neighborDistance;
                direction = (Direction)i;
            }
        }

        if (minDistanceToPlayer < ATTACK_DISTANCE)
            return GetAttackFromDirection(direction);
        else
            return GetWalkFromDirection(direction);
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

            if (GetPathMapDist(&neighborCell) > FLEE_DISTANCE_MIN && IsTerrainTraversable(&neighborCell)
                && !IsCellBlockedDiagonally(&neighborCell, (Direction)directionToMove))
            {
                return GetWalkFromDirection((Direction)directionToMove);
            }

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
    case ACTION_ADVANCE_FLOOR:
        actionCompleted = ActionAdvanceFloor();
        break;
    case ACTION_ATTACK_NORTH:
        actionCompleted = ActionAttack(actor, DIR_NORTH);
        break;
    case ACTION_ATTACK_EAST:
        actionCompleted = ActionAttack(actor, DIR_EAST);
        break;
    case ACTION_ATTACK_SOUTH:
        actionCompleted = ActionAttack(actor, DIR_SOUTH);
        break;
    case ACTION_ATTACK_WEST:
        actionCompleted = ActionAttack(actor, DIR_WEST);
        break;
    case ACTION_ATTACK_NORTH_EAST:
        actionCompleted = ActionAttack(actor, DIR_NORTH_EAST);
        break;
    case ACTION_ATTACK_SOUTH_EAST:
        actionCompleted = ActionAttack(actor, DIR_SOUTH_EAST);
        break;
    case ACTION_ATTACK_SOUTH_WEST:
        actionCompleted = ActionAttack(actor, DIR_SOUTH_WEST);
        break;
    case ACTION_ATTACK_NORTH_WEST:
        actionCompleted = ActionAttack(actor, DIR_NORTH_WEST);
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
    UpdateActorFacing(actor, direction);

    // Player-specific code.
    if (actor == GetActor(PLAYER_ACTOR_INDEX))
    {
        if (GetCellTerrain(&newPosition) == TERRAIN_STAIRS)
            ActionAdvanceFloor();
        else
            SetPathMapUpdateStatus(UPDATE_NEEDED);
    }

    return TRUE;
}

// ------------------------------------------------------------------------------------------------
// Attempts to ACTION_WALK the player in the direction of the next cell on the path to the
// selectedCell. Returns FALSE if the action fails.
static gboolean ActionWalkAuto(void)
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
    else
        SetSelectedCellStatus(STATUS_LOCKED);

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
// Advances the player to the next floor.
static gboolean ActionAdvanceFloor(void)
{
    GenerateDungeon();

    Actor *player = GetActor(0);
    SetActorHealthCurrent(player, GetActorHealthCurrent(player) + 5);
    CenterViewPortOn(&player->position);

    SetSelectedCellStatus(STATUS_OFF);
    SetActionForPlayer(ACTION_NONE);

    gtk_widget_queue_draw(GTK_WIDGET(viewPort));
    gtk_widget_queue_draw(GTK_WIDGET(menu));

    return TRUE;
}

// ------------------------------------------------------------------------------------------------
// Gets the Walk action associated with a given direction.
Action GetWalkFromDirection(Direction direction)
{
    return (Action)(ACTION_WALK_NORTH + (guint)direction);
}

// ------------------------------------------------------------------------------------------------
// Attempts to deal one damage to the target in the given direction.
// Returns FALSE if the action fails.
static gboolean ActionAttack(Actor *attacker, Direction direction)
{
    Point target = *GetActorPosition(attacker);
    target.x += hMovement[direction];
    target.y += vMovement[direction];

    if (IsOutsideDungeon(&target) || !IsVisibleToPlayer(&target) || !IsCellOccupiedByActor(&target))
        return FALSE;

    Actor *defender = GetCellsActor(&target);

    SetActorHealthCurrent(defender, GetActorHealthCurrent(defender) - 1);
    SetActorDamaged(defender, TRUE);

    return TRUE;
}

// ------------------------------------------------------------------------------------------------
// Gets the Attack action associated with a given direction.
Action GetAttackFromDirection(Direction direction)
{
    return (Action)(ACTION_ATTACK_NORTH + (guint)direction);
}

// ------------------------------------------------------------------------------------------------
// Attempts to change the terrain at the given actor's target position.
// Returns FALSE if the action fails.
static gboolean ActionTerrainFlip(Actor *actor, Point *target)
{
    Terrain terrain = GetCellTerrain(target);

    if (IsOutsideDungeon(target) || IsCellOccupiedByActor(target) || !IsVisibleToPlayer(target)
        || (terrain != TERRAIN_WALL && terrain != TERRAIN_FLOOR))
        return FALSE;

    if (terrain == TERRAIN_WALL)
        SetCellTerrain(target, TERRAIN_FLOOR);
    else if (terrain == TERRAIN_FLOOR)
        SetCellTerrain(target, TERRAIN_WALL);

    // If terrain was changed, the pathMap must be updated.
    SetPathMapUpdateStatus(UPDATE_NEEDED);

    return TRUE;
}
