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


// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

extern GtkDrawingArea *viewPort;
extern Point viewPosition;
extern Point selectedCell;

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
gboolean on_viewPort_update(GtkWidget *widget, cairo_t *context, gpointer userData);

#endif // VIEWPORT_H
