#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include "global.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------

#if KINDLE_BUILD
#define VIEWPORT_WIDTH  1216
#define VIEWPORT_HEIGHT 832
#else
#define VIEWPORT_WIDTH  608
#define VIEWPORT_HEIGHT 416
#endif

// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

extern GtkDrawingArea *viewPort;
extern Point viewPosition;
extern gboolean zoomIsOn;

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

void InitViewPort(void);
Point* GetViewPosition(void);
void SetViewPosition(Point *position);
void MoveViewPosition(Direction direction, guint distance);
void CenterViewPortOn(Point *position);
gboolean GetViewPortZoom(void);
void SetViewPortZoom(gboolean zoomIsOn);
gboolean IsPositionOnScreen(Point *position);

#endif // VIEWPORT_H
