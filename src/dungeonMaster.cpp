#include <cstdlib>
#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include "global.h"
#include "action.h"
#include "actor.h"
#include "dungeonMaster.h"

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
void ProcessTurns(void)
{
    gboolean actionCompleted = FALSE;

    for (guint i = 0; i < MAX_ACTOR_COUNT; i++)
    {
        Actor *actor = GetActor(i);

        if (actor->species == SPECIES_PLAYER)
        {
            actionCompleted = DoAction(actor, playerActions[CURRENT_ACTION]);

            // If player attempts action that cannot be completed, return without processing a turn.
            if (!actionCompleted)
            {
                playerActions[CURRENT_ACTION] = ACTION_NULL;
                return;
            }
        }
        else
        {
            // Randomly choose a direction for the non-player actor to move in.
            do
            {
                actionCompleted = DoAction(actor, (Action)(rand() % DIR_CARDINAL_COUNT + 1));
            } while (!actionCompleted);
        }
    }

    playerActions[CURRENT_ACTION] = ACTION_NULL;
}
