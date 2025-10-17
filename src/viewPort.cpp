#include <gtk-2.0/gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <glib-2.0/glib.h>
#include <cairo/cairo.h>
#include <cstdlib>
#include "actor.h"
#include "dungeonCell.h"
#include "viewPort.h"
#include "data/tile_null.h"
#include "data/tile_floor_base.h"
#include "data/tile_wall_bottom.h"
#include "data/tile_wall_top.h"
#include "data/tile_wall_right.h"
#include "data/tile_wall_top_right.h"
#include "data/tile_wall_left.h"
#include "data/tile_wall_top_left.h"
#include "data/tile_wall_right_left.h"
#include "data/tile_wall_top_right_left.h"
#include "data/tile_cell_selected.h"
#include "data/tile_at.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

extern const guint8 tile_null[];
extern const guint8 tile_floor_base[];
extern const guint8 tile_wall_top[];
extern const guint8 tile_wall_top_right[];
extern const guint8 tile_wall_right[];
extern const guint8 tile_wall_bottom[];
extern const guint8 tile_wall_left[];
extern const guint8 tile_wall_top_left[];
extern const guint8 tile_wall_right_left[];
extern const guint8 tile_wall_top_right_left[];
extern const guint8 tile_cell_selected[];
extern const guint8 tile_at[];

GdkPixbuf *tiles[TILE_COUNT] = {NULL};
GtkDrawingArea *viewPort = NULL;
Point viewPosition = {0}; // The dungeonCell position of the viewPort origin.
Point selectedCell = {0}; // The current player-selected dungeonCell in the viewPort.

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

static Tile GetWallTile(gint positionX, gint positionY);
static GdkPixbuf* GetTileForCellSelected(gint positionX, gint positionY);
static GdkPixbuf* GetTileForTerrain(gint positionX, gint positionY);
static GdkPixbuf* GetTileForCell(gint positionX, gint positionY);
static GdkPixbuf* GetPixbufFromTile(Tile tile);

// ------------------------------------------------------------------------------------------------
// Load GdkPixbuf tiles and initialize the dungeon viewPort.
void InitViewPort(void)
{
    LoadImagesToPixbufs();
    viewPort = GTK_DRAWING_AREA(gtk_drawing_area_new());
    gtk_widget_set_size_request(GTK_WIDGET(viewPort), VIEWPORT_WIDTH * TILE_SIZE, VIEWPORT_HEIGHT * TILE_SIZE);
}

// ------------------------------------------------------------------------------------------------
// Gets the dungeonCell position of the viewPort origin.
Point* GetViewPosition(void)
{
    return &viewPosition;
}

// ------------------------------------------------------------------------------------------------
// Sets the dungeonCell position of the viewPort origin to the given values.
void SetViewPosition(gint positionX, gint positionY)
{
    viewPosition.x = positionX;
    viewPosition.y = positionY;
}

// ------------------------------------------------------------------------------------------------
// Moves the dungeonCell position of the viewPort origin based on the given direction and distance.
void MoveViewPosition(Direction direction, guint distance)
{
    Point *position = GetViewPosition();

    switch (direction)
    {
    case DIR_UP:
        position->y -= distance;
        break;
    case DIR_DOWN:
        position->y += distance;
        break;
    case DIR_LEFT:
        position->x -= distance;
        break;
    case DIR_RIGHT:
        position->x += distance;
        break;
    default:
        break;
    }

    SetViewPosition(position->x, position->y);
}

// ------------------------------------------------------------------------------------------------
// Sets the dungeonCell position of the viewPort origin such that the given position is centered.
void CenterViewPortOn(gint positionX, gint positionY)
{
    SetViewPosition(positionX - VIEWPORT_WIDTH / 2, positionY - VIEWPORT_HEIGHT / 2);
}

// ------------------------------------------------------------------------------------------------
// Gets the dungeonCell position of the currently selected cell in the viewPort.
Point* GetSelectedCell(void)
{
    return &selectedCell;
}

// ------------------------------------------------------------------------------------------------
// Sets the currently selected dungeonCell in the viewPort to the given position.
void SetSelectedCell(gint positionX, gint positionY)
{
    selectedCell.x = positionX;
    selectedCell.y = positionY;
}

// ------------------------------------------------------------------------------------------------
// Returns the array of image data required for gdk_pixbuf_new_from_inline.
const guint8* GetTileData(Tile tile)
{
    switch (tile)
    {
    case TILE_NULL:
        return tile_null;
    case TILE_WALL_TOP:
        return tile_wall_top;
    case TILE_WALL_RIGHT:
        return tile_wall_right;
    case TILE_WALL_TOP_RIGHT:
        return tile_wall_top_right;
    case TILE_WALL_BOTTOM:
        return tile_wall_bottom;
    case TILE_WALL_LEFT:
        return tile_wall_left;
    case TILE_WALL_TOP_LEFT:
        return tile_wall_top_left;
    case TILE_WALL_RIGHT_LEFT:
        return tile_wall_right_left;
    case TILE_WALL_TOP_RIGHT_LEFT:
        return tile_wall_top_right_left;
    case TILE_FLOOR_BASE:
        return tile_floor_base;
    case TILE_CELL_SELECTED:
        return tile_cell_selected;
    case TILE_AT:
        return tile_at;
    case TILE_COUNT:
        return NULL;
    }

    return NULL;
}

// ------------------------------------------------------------------------------------------------
// Returns the tile image for a TERRAIN_WALL cell based on the surrounding cells.
static Tile GetWallTile(gint positionX, gint positionY)
{
    guint tileMask = 0;
    Terrain cellUp = GetCellTerrain(positionX, positionY - 1);
    Terrain cellRight = GetCellTerrain(positionX + 1, positionY);
    Terrain cellLeft = GetCellTerrain(positionX - 1, positionY);
    Terrain cellDown = GetCellTerrain(positionX, positionY + 1);
    Terrain cell45 = GetCellTerrain(positionX + 1, positionY - 1);
    Terrain cell135 = GetCellTerrain(positionX + 1, positionY + 1);
    Terrain cell225 = GetCellTerrain(positionX - 1, positionY - 1);
    Terrain cell315 = GetCellTerrain(positionX - 1, positionY + 1);

    if (cellUp == TERRAIN_FLOOR)
        tileMask +=1;
    if (cellRight == TERRAIN_FLOOR)
        tileMask +=2;
    if (cellDown == TERRAIN_FLOOR)
        tileMask +=4;
    if (cellLeft == TERRAIN_FLOOR)
        tileMask +=8;

    switch ((TileMask)tileMask)
    {
    case MASK_TOP:
        if (cell135 == TERRAIN_FLOOR && cell315 == TERRAIN_FLOOR)
            return TILE_WALL_TOP_RIGHT_LEFT;
        else if (cell135 == TERRAIN_FLOOR)
            return TILE_WALL_TOP_RIGHT;
        else if (cell315 == TERRAIN_FLOOR)
            return TILE_WALL_TOP_LEFT;
        else
            return TILE_WALL_TOP;
    case MASK_RIGHT:
        if (cell225 == TERRAIN_FLOOR && cell315 == TERRAIN_FLOOR)
            return TILE_WALL_RIGHT_LEFT;
        else
            return TILE_WALL_RIGHT;
    case MASK_BOTTOM:
    case MASK_TOP_BOTTOM:
    case MASK_TOP_RIGHT_BOTTOM:
    case MASK_TOP_BOTTOM_LEFT:
    case MASK_TOP_RIGHT_BOTTOM_LEFT:
    case MASK_RIGHT_BOTTOM_LEFT:
    case MASK_BOTTOM_LEFT:
    case MASK_RIGHT_BOTTOM:
        return TILE_WALL_BOTTOM;
    case MASK_LEFT:
        if (cell45 == TERRAIN_FLOOR && cell135 == TERRAIN_FLOOR)
            return TILE_WALL_RIGHT_LEFT;
        else
            return TILE_WALL_LEFT;
    case MASK_RIGHT_LEFT:
        return TILE_WALL_RIGHT_LEFT;
    case MASK_TOP_RIGHT:
        return TILE_WALL_TOP_RIGHT;
    case MASK_TOP_LEFT:
        return TILE_WALL_TOP_LEFT;
    case MASK_TOP_RIGHT_LEFT:
        return TILE_WALL_TOP_RIGHT_LEFT;
    default:
        if (cell135 == TERRAIN_FLOOR && cell315 == TERRAIN_FLOOR)
            return TILE_WALL_RIGHT_LEFT;
        else if (cell45 == TERRAIN_FLOOR && cell135 == TERRAIN_FLOOR)
            return TILE_WALL_RIGHT;
        else if (cell225 == TERRAIN_FLOOR && cell315 == TERRAIN_FLOOR)
            return TILE_WALL_LEFT;
        else
        return TILE_NULL;
    }
}

// ------------------------------------------------------------------------------------------------
// Returns the GdkPixbuf from the tiles array for the dungeonCell selected indicator.
static GdkPixbuf* GetTileForCellSelected(gint positionX, gint positionY)
{
    return GetPixbufFromTile(TILE_CELL_SELECTED);
}

// ------------------------------------------------------------------------------------------------
// Returns the GdkPixbuf from the tiles array for the given actor.
static GdkPixbuf* GetTileForActor(Actor *actor)
{
    ActorSpecies species = actor->species;
    Tile tile;

    switch (species)
    {
    case SPECIES_PLAYER:
        tile = TILE_AT;
        break;
    default:
        tile = TILE_NULL;
    }

    return GetPixbufFromTile(tile);
}

// ------------------------------------------------------------------------------------------------
// Returns the GdkPixbuf from the tiles array for the given cell based on its terrain.
static GdkPixbuf* GetTileForTerrain(gint positionX, gint positionY)
{
    Terrain terrain = GetCellTerrain(positionX, positionY);
    Tile tile;

    switch (terrain)
    {
    case TERRAIN_FLOOR:
        tile = TILE_FLOOR_BASE;
        break;
    case TERRAIN_WALL:
        tile = GetWallTile(positionX, positionY);
        break;
    default:
        tile = TILE_NULL;
    }

    return GetPixbufFromTile(tile);
}

// ------------------------------------------------------------------------------------------------
// Returns the GdkPixbuf from the tiles array based on the dungeonCell's contents.
static GdkPixbuf* GetTileForCell(gint positionX, gint positionY)
{
    DungeonCell *cellToDraw = GetCellAtPosition(positionX, positionY);
    Point *selectedCell = GetSelectedCell();

    if (IsOutsideDungeon(positionX, positionY))
        return GetPixbufFromTile(TILE_NULL);
    else if (selectedCell->x == positionX && selectedCell->y == positionY)
        return GetTileForCellSelected(positionX, positionY);
    else if (cellToDraw->actor != NULL)
        return GetTileForActor(cellToDraw->actor);
    else
        return GetTileForTerrain(positionX, positionY);

    return GetPixbufFromTile(TILE_NULL);
}

// ------------------------------------------------------------------------------------------------
// Returns a GdkPixbuf based on the given idex to the tiles array.
static GdkPixbuf* GetPixbufFromTile(Tile tile)
{
    return tiles[tile];
}

// ------------------------------------------------------------------------------------------------
// Read image data into the GdkPixbufs tiles array.
void LoadImagesToPixbufs(void)
{
    GError * error = NULL;
    for (guint i = 0; i < TILE_COUNT; i++)
    {
        tiles[i] = gdk_pixbuf_new_from_inline(-1, GetTileData((Tile)i), FALSE, &error);
        tiles[i] = gdk_pixbuf_scale_simple(tiles[i], TILE_SIZE, TILE_SIZE, GDK_INTERP_NEAREST);
    }
}

// ------------------------------------------------------------------------------------------------
// Free the GdkPixbufs for the tiles array.
void FreePixbufs(void)
{
    // Free memory used by GdkPixbufs.
    for (guint i = 0; i < TILE_COUNT; i++)
    {
        g_object_unref(tiles[i]);
    }
}

// ------------------------------------------------------------------------------------------------
// Callback function to update the tiles shown on the viewPort.
gboolean on_viewPort_update(GtkWidget *widget, cairo_t *context, gpointer userData)
{
    // Get the GdkWindow from the widget
    GdkWindow *window = gtk_widget_get_window(widget);

    if (window)
    {
        // Create a Cairo context from the GdkWindow
        cairo_t *context = gdk_cairo_create(window);
        Point *viewPosition = GetViewPosition();
        Point *selectedCell = GetSelectedCell();

        for (gint y = 0; y < VIEWPORT_HEIGHT; y++)
        {
            for (gint x = 0; x < VIEWPORT_WIDTH; x++)
            {
                // The pixel position within the viewPort to be changed.
                gint pixelX = TILE_SIZE * x;
                gint pixelY = TILE_SIZE * y;

                // The dungeon cell to be drawn in the viewPort.
                gint cellX = viewPosition->x + x;
                gint cellY = viewPosition->y + y;

                // Draws the terrain for the cell.
                gdk_cairo_set_source_pixbuf(context, GetTileForTerrain(cellX, cellY), pixelX, pixelY);
                cairo_paint(context);

                // If position contains an actor, draw it over the terrain.
                if (GetCellsActor(cellX, cellY) != NULL)
                {
                    gdk_cairo_set_source_pixbuf(context, tiles[TILE_AT], pixelX, pixelY);
                    cairo_paint(context);
                }

                // If position is also the selected cell, draw the cursor over everything else.
                if (selectedCell->x == cellX && selectedCell->y == cellY)
                {
                    gdk_cairo_set_source_pixbuf(context, tiles[TILE_CELL_SELECTED], pixelX, pixelY);
                    cairo_paint(context);
                }
            }
        }

        // Clean up the Cairo context
        cairo_destroy(context);
    }
    return FALSE;
}

// ------------------------------------------------------------------------------------------------
// Callback function to track input on the viewPort.
gboolean on_viewPort_click(GtkWidget *widget, GdkEventButton *event, gpointer userData)
{
    Point clickedTile = {0};
    Point *viewPosition = GetViewPosition();
    Point *oldSelectedCell = GetSelectedCell();
    Point newSelectedCell = {0};

    // Get pixbuf tile that was clicked.
    clickedTile.x = (gint)(event->x / TILE_SIZE);
    clickedTile.y = (gint)(event->y / TILE_SIZE);

    // Get the dungeon cell of the clicked tile.
    newSelectedCell.x = viewPosition->x + clickedTile.x;
    newSelectedCell.y = viewPosition->y + clickedTile.y;

    // Center viewPort on selected dungeon cell if it was clicked again.
    if (newSelectedCell.x == oldSelectedCell->x && newSelectedCell.y == oldSelectedCell->y)
    {
        CenterViewPortOn(newSelectedCell.x, newSelectedCell.y);
        SetActorPosition(&actors[0], newSelectedCell.x, newSelectedCell.y);
    }
    else
    {
        SetSelectedCell(newSelectedCell.x, newSelectedCell.y);
    }

    // Queue update to the viewPort.
    gtk_widget_queue_draw(GTK_WIDGET(viewPort));

    return TRUE;
}
