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
    TILE_ZERO,
    TILE_ONE,
    TILE_COUNT
};

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
const guint8* GetTileData(enum TILE tile);
void LoadImagesToPixbufs(void);
void FreePixbufs(void);

#endif // VIEWPORT_H
