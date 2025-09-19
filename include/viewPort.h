#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------

#define VIEWPORT_WIDTH  25
#define VIEWPORT_HEIGHT 15
#define SCALE_SIZE 48

// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------

enum TILE
{
    TILE_NULL,
    TILE_ZERO,
    TILE_ONE,
    TILE_COUNT
};

enum DIRECTION
{
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT,
    DIR_COUNT
};

typedef struct
{
    gint x;
    gint y;
} Point;

// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

extern GtkTable *viewPort;
extern GtkImage *viewPieces[VIEWPORT_WIDTH * VIEWPORT_HEIGHT];
extern GdkPixbuf *tiles[TILE_COUNT];

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

void InitViewPort(void);
void UpdateViewPieces(void);
Point* GetViewPosition(void);
void SetViewPosition(gint positionX, gint positionY);
void MoveViewPosition(DIRECTION direction, guint distance);
const guint8* GetTileData(enum TILE tile);
void LoadImagesToPixbufs(void);
void FreePixbufs(void);

#endif // VIEWPORT_H
