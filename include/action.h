#ifndef ACTION_H
#define ACTION_H

#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include "global.h"
#include "actor.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------

#define MAX_QUEUED_PLAYER_ACTIONS  1
#define CURRENT_ACTION             0

// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------

enum Action
{
    ACTION_NULL,
    ACTION_WALK_NORTH,
    ACTION_WALK_EAST,
    ACTION_WALK_SOUTH,
    ACTION_WALK_WEST,
    ACTION_COUNT
};

// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

extern Action playerActions[MAX_QUEUED_PLAYER_ACTIONS];

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

Action GetQueuedPlayerAction(guint queueIndex);
void SetQueuedPlayerAction(guint queueIndex, Action action);
void ClearQueuedPlayerActions(void);
gboolean DoAction(Actor *actor, Action action);

#endif // ACTION_H
