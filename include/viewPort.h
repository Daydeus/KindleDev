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
#define VIEWPORT_WIDTH  704
#define VIEWPORT_HEIGHT 448
#endif

// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------

enum ViewPortMode
{
    MODE_CHARACTER,
    MODE_SELECTOR,
    MODE_COUNT
};

// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

extern GtkDrawingArea *viewPort;
extern Point viewPosition;
extern Point selectedCell;
extern ViewPortMode viewPortMode;
extern gboolean zoomIsOn;

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

void InitViewPort(void);
Point* GetViewPosition(void);
void SetViewPosition(Point *position);
void MoveViewPosition(Direction direction, guint distance);
void CenterViewPortOn(Point *position);
Point* GetSelectedCell(void);
void SetSelectedCell(Point *position);
gboolean GetViewPortZoom(void);
void SetViewPortZoom(gboolean zoomIsOn);
ViewPortMode GetViewPortMode(void);
void SetViewPortMode(ViewPortMode mode);

#endif // VIEWPORT_H
