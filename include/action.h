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

// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------

enum Action
{
    ACTION_NONE,
    ACTION_WALK_NORTH,
    ACTION_WALK_EAST,
    ACTION_WALK_SOUTH,
    ACTION_WALK_WEST,
    ACTION_WALK_NORTH_EAST,
    ACTION_WALK_SOUTH_EAST,
    ACTION_WALK_SOUTH_WEST,
    ACTION_WALK_NORTH_WEST,
    ACTION_WALK_AUTO,
    ACTION_COUNT
};

// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

extern Action playerAction;
extern gboolean inputIsBlocked;

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

Action GetActionForPlayer(void);
void SetActionForPlayer(Action action);
gboolean GetInputBlockStatus(void);
void SetInputBlockStatus(gboolean isBlocked);
Action GetActionForAI(void);
gboolean DoAction(Actor *actor, Action action);
Action GetWalkFromDirection(Direction direction);

#endif // ACTION_H
