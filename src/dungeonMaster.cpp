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


// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Loop through all actors and have them perform an action for their turn.
// If return value is true, ProcessTurn will be called again after a delay.
gboolean ProcessTurn(gpointer data)
{
    for (gint i = PLAYER_ACTOR_INDEX; i < MAX_ACTOR_COUNT; i++)
    {
        Actor *actor = GetActor(i);
        Action action = ACTION_NONE;
        gboolean actionCompleted = FALSE;

        // Get action the actor will perform.
        if (i == PLAYER_ACTOR_INDEX)
            action = GetActionForPlayer();
        else if (IsActorDead(actor) == TRUE)
            continue;
        else
            action = GetActionForAI(actor);

        // Attempt to complete action and store result.
        actionCompleted = DoAction(actor, action);

        // Player is always first to act.
        if (i == PLAYER_ACTOR_INDEX)
        {
            // If the player's action failed, do not process the turn.
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

        // If actor is (or was) visible to player, pause processing to redraw the viewPort mid-turn.
        if (IsVisibleToPlayer(&actor->prevPosition) || IsVisibleToPlayer(&actor->position))
        {
            gtk_widget_queue_draw(GTK_WIDGET(viewPort));
            WaitForScreenRedraw();
        }

        // Rebuild pathMap with the player as origin if update is needed (player changed position, etc).
        if (GetPathMapUpdateStatus() == UPDATE_NEEDED)
        {
            SetPathMapOrigin(GetActorPosition(GetActor(PLAYER_ACTOR_INDEX)));
            BuildPathMap();
            SetPathMapUpdateStatus(UPDATE_NOT_NEEDED);
        }
    }

    // Update the miniMap after all actors have taken their turn.
    if (GetMenuState() == STATE_MINIMAP)
        gtk_widget_queue_draw(GTK_WIDGET(menu));
    gtk_widget_queue_draw(GTK_WIDGET(viewPort));

    // End game if player was killed.
    if (IsActorDead(GetActor(PLAYER_ACTOR_INDEX)))
    {
        g_print("Player was killed.\n");
        gtk_main_quit();
    }

    // If player is auto-navigating and has not arrived yet, call ProcessTurn again.
    if (GetActionForPlayer() == ACTION_WALK_AUTO
        && !IsSamePoint(GetActorPosition(GetActor(PLAYER_ACTOR_INDEX)), GetSelectedCell()))
    {
        return TRUE;
    }

    SetActionForPlayer(ACTION_NONE);
    return FALSE;
}
