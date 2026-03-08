#include <cstdlib>
#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include "global.h"
#include "action.h"
#include "actor.h"
#include "dungeonMaster.h"
#include "fieldOfView.h"
#include "menu.h"
#include "pathfinding.h"
#include "viewPort.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

guint turnCount = 0;
guint nextTurnActorIndex = 0;

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

static guint GetNextTurnActorIndex(void);
static void SetNextTurnActorIndex(guint value);

// ------------------------------------------------------------------------------------------------
// Returns the number of turns passed.
guint GetTurnCount(void)
{
    return turnCount;
}

// ------------------------------------------------------------------------------------------------
// Sets the number of turns passed to zero.
void ClearTurnCount(void)
{
    turnCount = 0;
}

// ------------------------------------------------------------------------------------------------
// Loop through all actors and have them perform an action for their turn.
// If return value is true, ProcessTurn will be called again after a delay.
gboolean ProcessTurn(gpointer data)
{
    guint nextActor = GetNextTurnActorIndex();

    // Loop through each actor and perform their action.
    for (guint i = nextActor; i < MAX_ACTOR_COUNT; i++)
    {
        Actor *actor = GetActor(i);
        Action action = ACTION_NONE;
        gboolean actionCompleted = FALSE;

        // Skip turn if actor is not alive.
        if (IsActorDead(actor))
        {
            // If dead actor is the final in list, set next actor to be the player.
            if (i >= MAX_ACTOR_COUNT - 1)
            {
                SetNextTurnActorIndex(PLAYER_ACTOR_INDEX);
                break;
            }
            else
            {
                SetNextTurnActorIndex(i + 1);
                continue;
            }
        }

        // Get action the actor will perform.
        if (i == PLAYER_ACTOR_INDEX)
            action = GetActionForPlayer();
        else
            action = GetActionForAI(actor);

        // Attempt to complete action and store result.
        actionCompleted = DoAction(actor, action);

        // Player-specific turn code.
        if (i == PLAYER_ACTOR_INDEX)
        {
            // If the player's action failed, abort processing the turn.
            if (!actionCompleted)
            {
                SetActionForPlayer(ACTION_NONE);
                return FALSE;
            }
            else
            {
                SetPlayerSightId(GetPlayerSightId() + 1);
                UpdateFOV(GetActorPosition(actor), GetActorSightRange(actor));
                CenterViewPortOn(&actor->position);
            }
        }

        // Rebuild pathMap with the player as origin if update is needed (player changed position, etc).
        if (GetPathMapUpdateStatus() == UPDATE_NEEDED)
        {
            SetPathMapOrigin(GetActorPosition(GetActor(PLAYER_ACTOR_INDEX)));
            BuildPathMap();
            SetPathMapUpdateStatus(UPDATE_NOT_NEEDED);
        }

        SetNextTurnActorIndex(i + 1);
        if (GetNextTurnActorIndex() >= MAX_ACTOR_COUNT - 1)
            SetNextTurnActorIndex(PLAYER_ACTOR_INDEX);

        // If actor is (or was) visible to player, interrupt processing turns so screen can be redrawn.
        if (IsVisibleToPlayer(&actor->prevPosition) || IsVisibleToPlayer(&actor->position))
            break;
    }

    // Update the screen.
    gtk_widget_queue_draw(GTK_WIDGET(viewPort));
    gtk_widget_queue_draw(GTK_WIDGET(menu));

    // End game if player was killed.
    if (IsActorDead(GetActor(PLAYER_ACTOR_INDEX)))
        gtk_main_quit();

    // Call ProcessTurn again after returning to let the screen update.
    if (GetNextTurnActorIndex() != PLAYER_ACTOR_INDEX)
    {
        return TRUE;
    }
    else if (GetActionForPlayer() == ACTION_WALK_AUTO && !IsSamePoint(GetActorPosition(GetActor(PLAYER_ACTOR_INDEX)), GetSelectedCell()))
    {
        turnCount++;
        return TRUE;
    }
    else // All actors' turns processed.
    {
        SetActionForPlayer(ACTION_NONE);
        turnCount++;
        return FALSE;
    }
}

// ------------------------------------------------------------------------------------------------
// Returns the actor index of the actor whose turn it is.
static guint GetNextTurnActorIndex(void)
{
    return nextTurnActorIndex;
}

// ------------------------------------------------------------------------------------------------
// Sets the actor index of the actor whose turn it is.
static void SetNextTurnActorIndex(guint value)
{
    nextTurnActorIndex = value;
}
