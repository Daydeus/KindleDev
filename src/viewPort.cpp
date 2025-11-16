#include <gtk-2.0/gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <glib-2.0/glib.h>
#include <cairo/cairo.h>
#include <cstdlib>
#include "global.h"
#include "actor.h"
#include "dungeonCell.h"
#include "viewPort.h"
#include "data/tiles/tile_at.h"
#include "data/tiles/tile_cell_selected.h"
#include "data/tiles/tile_floor.h"
#include "data/tiles/tile_null.h"
#include "data/tiles/tile_wall_cave_dualCorners_northWestToSouthEast.h"
#include "data/tiles/tile_wall_cave_dualCorners_southWestToNorthEast.h"
#include "data/tiles/tile_wall_cave_facing_east.h"
#include "data/tiles/tile_wall_cave_facing_north.h"
#include "data/tiles/tile_wall_cave_facing_south.h"
#include "data/tiles/tile_wall_cave_facing_west.h"
#include "data/tiles/tile_wall_cave_innerCorner_northEast.h"
#include "data/tiles/tile_wall_cave_innerCorner_northWest.h"
#include "data/tiles/tile_wall_cave_innerCorner_southEast.h"
#include "data/tiles/tile_wall_cave_innerCorner_southWest.h"
#include "data/tiles/tile_wall_cave_outerCorner_northEast.h"
#include "data/tiles/tile_wall_cave_outerCorner_northWest.h"
#include "data/tiles/tile_wall_cave_outerCorner_southEast.h"
#include "data/tiles/tile_wall_cave_outerCorner_southWest.h"
#include "data/tiles/tile_wall_cave_standalone.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

GdkPixbuf *tiles[TILE_COUNT] = {NULL};
GtkDrawingArea *viewPort = NULL;
ZoomLevel zoomLevel = {ZOOM_LEVEL_OFF};
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
static const guint8* GetTileImageData(Tile tile);
static guint GetTileSizeForZoomLevel(ZoomLevel level);

// ------------------------------------------------------------------------------------------------
// Load GdkPixbuf tiles and initialize the dungeon viewPort.
void InitViewPort(void)
{
    LoadTiles();

    // Initialize the viewPort.
    viewPort = GTK_DRAWING_AREA(gtk_drawing_area_new());
    gtk_widget_set_size_request(GTK_WIDGET(viewPort), VIEWPORT_WIDTH_PIXELS, VIEWPORT_HEIGHT_PIXELS);
    SetWidgetBgColor(GTK_WIDGET(viewPort), COLOR_WHITE);

    // Set up signals.
    g_signal_connect(viewPort, "expose_event", G_CALLBACK(on_viewPort_update), NULL);
    g_signal_connect(viewPort, "button_press_event", G_CALLBACK(on_viewPort_click), NULL);
    gtk_widget_set_events(GTK_WIDGET(viewPort), GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK);
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

    position->x += hMovement[direction] * distance;
    position->y += vMovement[direction] * distance;

    SetViewPosition(position->x, position->y);
}

// ------------------------------------------------------------------------------------------------
// Sets the dungeonCell position of the viewPort origin such that the given position is centered.
void CenterViewPortOn(gint positionX, gint positionY)
{
    guint tileSize = GetTileSizeForZoomLevel(GetViewPortZoomLevel());
    guint viewPortWidth = VIEWPORT_WIDTH_PIXELS / tileSize;
    guint viewPortHeight = VIEWPORT_HEIGHT_PIXELS / tileSize;

    SetViewPosition(positionX - viewPortWidth / 2, positionY - viewPortHeight / 2);
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
static const guint8* GetTileImageData(Tile tile)
{
    switch (tile)
    {
    case TILE_NULL:
        return tile_null;
    case TILE_WALL_DUAL_CORNERS_NORTHWEST_SOUTHEAST:
        return tile_wall_cave_dualCorners_northWestToSouthEast;
    case TILE_WALL_DUAL_CORNERS_SOUTHWEST_NORTHEAST:
        return tile_wall_cave_dualCorners_southWestToNorthEast;
    case TILE_WALL_FACING_EAST:
        return tile_wall_cave_facing_east;
    case TILE_WALL_FACING_NORTH:
        return tile_wall_cave_facing_north;
    case TILE_WALL_FACING_SOUTH:
        return tile_wall_cave_facing_south;
    case TILE_WALL_FACING_WEST:
        return tile_wall_cave_facing_west;
    case TILE_WALL_INNER_CORNER_NORTHEAST:
        return tile_wall_cave_innerCorner_northEast;
    case TILE_WALL_INNER_CORNER_NORTHWEST:
        return tile_wall_cave_innerCorner_northWest;
    case TILE_WALL_INNER_CORNER_SOUTHEAST:
        return tile_wall_cave_innerCorner_southEast;
    case TILE_WALL_INNER_CORNER_SOUTHWEST:
        return tile_wall_cave_innerCorner_southWest;
    case TILE_WALL_OUTER_CORNER_NORTHEAST:
        return tile_wall_cave_outerCorner_northEast;
    case TILE_WALL_OUTER_CORNER_NORTHWEST:
        return tile_wall_cave_outerCorner_northWest;
    case TILE_WALL_OUTER_CORNER_SOUTHEAST:
        return tile_wall_cave_outerCorner_southEast;
    case TILE_WALL_OUTER_CORNER_SOUTHWEST:
        return tile_wall_cave_outerCorner_southWest;
    case TILE_WALL_STANDALONE:
        return tile_wall_cave_standalone;
    case TILE_FLOOR:
        return tile_floor;
    case TILE_AT:
        return tile_at;
    case TILE_CELL_SELECTED:
        return tile_cell_selected;
    case TILE_COUNT:
        return NULL;
    }

    return NULL;
}

// ------------------------------------------------------------------------------------------------
// Returns the tile image for a TERRAIN_WALL cell based on the surrounding cells.
static Tile GetWallTile(gint positionX, gint positionY)
{
    guint tile = TILE_NULL;
    Terrain neighbors[DIR_COUNT] = {TERRAIN_NULL};

    // Get terrain for each neighboring cell.
    for (guint i = 0; i < DIR_COUNT; i++)
    {
        gint cellX = positionX + hMovement[i];
        gint cellY = positionY + vMovement[i];

        neighbors[i] = GetCellTerrain(cellX, cellY);
    }

    // Check corner neighbors for TERRAIN_FLOOR.
    if (neighbors[DIR_NORTH_EAST] == TERRAIN_FLOOR)
        tile |= MASK_NORTH_EAST;
    if (neighbors[DIR_SOUTH_EAST] == TERRAIN_FLOOR)
        tile |= MASK_SOUTH_EAST;
    if (neighbors[DIR_SOUTH_WEST] == TERRAIN_FLOOR)
        tile |= MASK_SOUTH_WEST;
    if (neighbors[DIR_NORTH_WEST] == TERRAIN_FLOOR)
        tile |= MASK_NORTH_WEST;

    // Check edge neighbors for TERRAIN_FLOOR.
    if (neighbors[DIR_NORTH] == TERRAIN_FLOOR)
    {
        tile |= MASK_NORTH_EAST;
        tile |= MASK_NORTH_WEST;
    }
    if (neighbors[DIR_EAST] == TERRAIN_FLOOR)
    {
        tile |= MASK_NORTH_EAST;
        tile |= MASK_SOUTH_EAST;
    }
    if (neighbors[DIR_SOUTH] == TERRAIN_FLOOR)
    {
        tile |= MASK_SOUTH_EAST;
        tile |= MASK_SOUTH_WEST;
    }
    if (neighbors[DIR_WEST] == TERRAIN_FLOOR)
    {
        tile |= MASK_SOUTH_WEST;
        tile |= MASK_NORTH_WEST;
    }

    return (Tile)tile;
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
        tile = TILE_FLOOR;
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
void LoadTiles(void)
{
    GError * error = NULL;
    for (guint i = 0; i < TILE_COUNT; i++)
    {
        tiles[i] = gdk_pixbuf_new_from_inline(-1, GetTileImageData((Tile)i), FALSE, &error);
    }
}

// ------------------------------------------------------------------------------------------------
// Scale the GdkPixbuf tiles based on the viewPort's zoomLevel.
void ScaleTileForZoom(void)
{
    guint tileSize = GetTileSizeForZoomLevel(GetViewPortZoomLevel());
    for (guint i = 0; i < TILE_COUNT; i++)
    {
        tiles[i] = gdk_pixbuf_scale_simple(tiles[i], tileSize, tileSize, GDK_INTERP_NEAREST);
    }
}

// ------------------------------------------------------------------------------------------------
// Free the GdkPixbufs for the tiles array.
void FreeTiles(void)
{
    // Free memory used by GdkPixbufs.
    for (guint i = 0; i < TILE_COUNT; i++)
    {
        g_object_unref(tiles[i]);
    }
}

// ------------------------------------------------------------------------------------------------
// Get whether zoom is active on the viewPort.
ZoomLevel GetViewPortZoomLevel(void)
{
    return zoomLevel;
}

// ------------------------------------------------------------------------------------------------
// Set whether zoom is active on the viewPort.
void SetViewPortZoomLevel(ZoomLevel level)
{
    zoomLevel = level;
}

// ------------------------------------------------------------------------------------------------
// Get whether zoom is active on the viewPort.
static guint GetTileSizeForZoomLevel(ZoomLevel level)
{
    switch(level)
    {
    case ZOOM_LEVEL_OFF:
        return TILE_SIZE_16;
    case ZOOM_LEVEL_MID:
        return TILE_SIZE_32;
    case ZOOM_LEVEL_PEAK:
        return TILE_SIZE_64;
    default:
        return TILE_SIZE_16;
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

        ZoomLevel zoomLevel = GetViewPortZoomLevel();
        gint tileSize = GetTileSizeForZoomLevel(zoomLevel);
        Point *viewPosition = GetViewPosition();
        Point *selectedCell = GetSelectedCell();

        for (gint y = 0; y <= VIEWPORT_HEIGHT_PIXELS / tileSize; y++)
        {
            for (gint x = 0; x <= VIEWPORT_WIDTH_PIXELS / tileSize; x++)
            {
                // The pixel position within the viewPort to be changed.
                gint pixelX = tileSize * x;
                gint pixelY = tileSize * y;

                // Zoom levels other than ZOOM_LEVEL_PEAK have an even number of tiles to draw,
                // so we draw them offset by half a tile to keep the player centered.
                if (zoomLevel != ZOOM_LEVEL_PEAK)
                {
                    pixelX -= tileSize / 2;
                    pixelY -= tileSize / 2;
                }

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
    gint tileSize = GetTileSizeForZoomLevel(GetViewPortZoomLevel());
    Point clickedTile = {0};
    Point *viewPosition = GetViewPosition();
    Point *oldSelectedCell = GetSelectedCell();
    Point newSelectedCell = {0};

    // Zoom levels other than ZOOM_LEVEL_PEAK have an even number of tiles to draw,
    // so we draw them offset by half a tile to keep the player centered.
    if (zoomLevel == ZOOM_LEVEL_PEAK)
    {
        clickedTile.x = (gint)(event->x / tileSize);
        clickedTile.y = (gint)(event->y / tileSize);
    }
    else
    {
        clickedTile.x = (gint)((event->x + tileSize / 2) / tileSize);
        clickedTile.y = (gint)((event->y + tileSize / 2) / tileSize);
    }

    // Get the dungeon cell of the clicked tile.
    newSelectedCell.x = viewPosition->x + clickedTile.x;
    newSelectedCell.y = viewPosition->y + clickedTile.y;

    // Center viewPort on selected dungeon cell if it was clicked again and is traversable.
    if (newSelectedCell.x == oldSelectedCell->x && newSelectedCell.y == oldSelectedCell->y)
    {
        if (IsTerrainTraversable(newSelectedCell.x, newSelectedCell.y))
        {
            CenterViewPortOn(newSelectedCell.x, newSelectedCell.y);
            SetActorPosition(&actors[0], newSelectedCell.x, newSelectedCell.y);
        }
    }
    else
    {
        SetSelectedCell(newSelectedCell.x, newSelectedCell.y);
    }

    // Queue update to the viewPort.
    gtk_widget_queue_draw(GTK_WIDGET(viewPort));

    return TRUE;
}
