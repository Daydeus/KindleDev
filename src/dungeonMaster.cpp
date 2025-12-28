#include <cstdlib>
#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include "global.h"
#include "action.h"
#include "actor.h"
#include "dungeonMaster.h"
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
void ProcessTurn(void)
{
    SetInputBlockStatus(INPUT_IS_BLOCKED);

    for (gint i = PLAYER_ACTOR_INDEX; i < MAX_ACTOR_COUNT; i++)
    {
        Actor *actor = GetActor(i);
        Action action = ACTION_NULL;
        gboolean actionCompleted = FALSE;

        if (i == PLAYER_ACTOR_INDEX)
            action = GetQueuedPlayerAction();
        else
            action = GetActionForAI();

        actionCompleted = DoAction(actor, action);

        if (i == PLAYER_ACTOR_INDEX)
        {
            if (!actionCompleted)
            {
                SetInputBlockStatus(INPUT_IS_NOT_BLOCKED);
                return;
            }
            else
                CenterViewPortOn(&actor->position);
        }

        // Redraw viewPort mid-turn if actor is on-screen.
        if (IsPositionOnScreen(&actor->position))
        {
            gtk_widget_queue_draw(GTK_WIDGET(viewPort));
            WaitForScreenRedraw();
        }
    }

    SetInputBlockStatus(INPUT_IS_NOT_BLOCKED);
}
