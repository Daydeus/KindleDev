#ifndef ACTION_H
#define ACTION_H

#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include "global.h"
#include "actor.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------

#define INPUT_IS_BLOCKED           TRUE
#define INPUT_IS_NOT_BLOCKED       FALSE
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

extern gboolean inputIsBlocked;
extern Action playerActions[MAX_QUEUED_PLAYER_ACTIONS];

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

gboolean GetInputBlockStatus(void);
void SetInputBlockStatus(gboolean isBlocked);
Action GetQueuedPlayerAction(void);
void SetQueuedPlayerAction(guint queueIndex, Action action);
void ClearQueuedPlayerActions(void);
Action GetActionForAI(void);
gboolean DoAction(Actor *actor, Action action);

#endif // ACTION_H
