#ifndef GLOBAL_H
#define GLOBAL_H

#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------

#define SCREEN_WIDTH_PIXELS  1264
#define SCREEN_HEIGHT_PIXELS 1680

// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------

// Colors available for the application to use.
enum Color
{
    COLOR_BLACK,
    COLOR_GREY_DARK,
    COLOR_GREY_LIGHT,
    COLOR_WHITE,
    COLOR_COUNT
};

enum Direction
{
    DIR_NORTH,
    DIR_NORTH_EAST,
    DIR_EAST,
    DIR_SOUTH_EAST,
    DIR_SOUTH,
    DIR_SOUTH_WEST,
    DIR_WEST,
    DIR_NORTH_WEST,
    DIR_COUNT,
    DIR_NONE = DIR_COUNT
};

typedef struct
{
    gint x;
    gint y;
} Point;

// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

extern const gint8 hMovement[DIR_COUNT];
extern const gint8 vMovement[DIR_COUNT];

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

gboolean IsWithinRectangle(Point *position, Point *origin, gint width, gint height);
void SetWidgetBgColor(GtkWidget *widget, enum Color colorName);

#endif // GLOBAL_H
