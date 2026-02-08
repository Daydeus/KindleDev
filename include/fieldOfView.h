#ifndef FIELD_OF_VIEW_H
#define FIELD_OF_VIEW_H

#include <glib-2.0/glib.h>
#include "global.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

extern guint playerSightId;
extern gboolean debug_fogOfWarIsOn;

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

guint GetPlayerSightId(void);
void SetPlayerSightId(guint newValue);
void UpdateFOV(Point *center, gint sightRange);
gboolean IsVisibleToPlayer(Point *position);
void PrintFOV(void);
gboolean GetFogOfWarStatus(void);
void SetFogOfWarStatus(gboolean status);

#endif // FIELD_OF_VIEW_H
