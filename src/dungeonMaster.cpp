#include <cstdlib>
#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include "global.h"
#include "action.h"
#include "actor.h"
#include "dungeonMaster.h"
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
    SetInputBlockStatus(INPUT_IS_BLOCKED);

    for (gint i = PLAYER_ACTOR_INDEX; i < MAX_ACTOR_COUNT; i++)
    {
        Actor *actor = GetActor(i);
        Action action = ACTION_NULL;
        gboolean actionCompleted = FALSE;

        // Get action the actor will perform.
        if (i == PLAYER_ACTOR_INDEX)
        {
            if (GetSelectedCellStatus() == STATUS_LOCKED)
                action = GetPlayerNavigation();
        }
        else
        {
            action = GetActionForAI();
        }

        // Attempt to complete action and store result.
        actionCompleted = DoAction(actor, action);

        // Player is always first to act.
        if (i == PLAYER_ACTOR_INDEX)
        {
            // If the player's action failed, do not process the turn.
            if (!actionCompleted)
            {
                SetInputBlockStatus(INPUT_IS_NOT_BLOCKED);
                return FALSE;
            }
            else
            {
                CenterViewPortOn(&actor->position);
            }
        }

        // If actor is on-screen, pause processing to redraw the viewPort mid-turn.
        if (IsPositionOnScreen(&actor->position))
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

    SetInputBlockStatus(INPUT_IS_NOT_BLOCKED);

    // Automatically call ProcessTurn again if player's next action is pre-determined (such as due to
    // auto-player navigation).
    if (GetSelectedCellStatus() == STATUS_LOCKED)
        return TRUE;
    else
        return FALSE;
}
