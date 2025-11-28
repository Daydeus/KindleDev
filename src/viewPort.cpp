#include <gtk-2.0/gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <glib-2.0/glib.h>
#include <cairo/cairo.h>
#include <cstdlib>
#include "global.h"
#include "touchGesture.h"
#include "actor.h"
#include "dungeonCell.h"
#include "viewPort.h"
#include "data/tilesetDungeonCave.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

GdkPixbuf *dungeonTiles[TILE_COUNT] = {NULL};
GtkDrawingArea *viewPort = NULL;
ZoomLevel zoomLevel = {ZOOM_LEVEL_OFF};
Point viewPosition = {0}; // The dungeonCell position of the viewPort origin.
Point selectedCell = {0}; // The current player-selected dungeonCell in the viewPort.

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

static DungeonTile GetWallTile(Point *position);
static GdkPixbuf* GetTileForTerrain(Point *position);
static GdkPixbuf* GetPixbufFromTile(DungeonTile tile);
static const guint8* GetTilesetImageData(DungeonTileset tileset);
static guint GetTileSizeForZoomLevel(ZoomLevel level);
static gboolean on_viewPort_click_press(GtkWidget *widget, GdkEventButton *event, gpointer userData);
static gboolean on_viewPort_click_release(GtkWidget *widget, GdkEventButton *event, gpointer userData);

// ------------------------------------------------------------------------------------------------
// Load GdkPixbuf tiles and initialize the dungeon viewPort.
void InitViewPort(void)
{
    LoadDungeonTiles(TILESET_CAVE);

    // Initialize the viewPort.
    viewPort = GTK_DRAWING_AREA(gtk_drawing_area_new());
    gtk_widget_set_size_request(GTK_WIDGET(viewPort), VIEWPORT_WIDTH_PIXELS, VIEWPORT_HEIGHT_PIXELS);
    SetWidgetBgColor(GTK_WIDGET(viewPort), COLOR_WHITE);

    // Set up signals.
    g_signal_connect(viewPort, "expose_event", G_CALLBACK(on_viewPort_update), NULL);
    g_signal_connect(viewPort, "button_press_event", G_CALLBACK(on_viewPort_click_press), NULL);
    g_signal_connect(viewPort, "button_release_event", G_CALLBACK(on_viewPort_click_release), NULL);
    gtk_widget_set_events(GTK_WIDGET(viewPort), GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK
                        | GDK_BUTTON_RELEASE_MASK);
}

// ------------------------------------------------------------------------------------------------
// Gets the dungeonCell position of the viewPort origin.
Point* GetViewPosition(void)
{
    return &viewPosition;
}

// ------------------------------------------------------------------------------------------------
// Sets the dungeonCell position of the viewPort origin to the given values.
void SetViewPosition(Point *position)
{
    viewPosition = *position;
}

// ------------------------------------------------------------------------------------------------
// Moves the dungeonCell position of the viewPort origin based on the given direction and distance.
void MoveViewPosition(Direction direction, guint distance)
{
    Point *position = GetViewPosition();

    position->x += hMovement[direction] * distance;
    position->y += vMovement[direction] * distance;

    SetViewPosition(position);
}

// ------------------------------------------------------------------------------------------------
// Sets the dungeonCell position of the viewPort origin such that the given position is centered.
void CenterViewPortOn(Point *position)
{
    guint tileSize = GetTileSizeForZoomLevel(GetViewPortZoomLevel());
    gint viewPortWidth = VIEWPORT_WIDTH_PIXELS / tileSize;
    gint viewPortHeight = VIEWPORT_HEIGHT_PIXELS / tileSize;
    // TODO: Simplify by making above variables into viewPortWidthHalf and viewPortHeightHalf
    Point newPosition = {position->x - viewPortWidth / 2, position->y - viewPortHeight / 2};

    SetViewPosition(&newPosition);
}

// ------------------------------------------------------------------------------------------------
// Gets the dungeonCell position of the currently selected cell in the viewPort.
Point* GetSelectedCell(void)
{
    return &selectedCell;
}

// ------------------------------------------------------------------------------------------------
// Sets the currently selected dungeonCell in the viewPort to the given position.
void SetSelectedCell(Point *position)
{
    selectedCell = *position;
}

// ------------------------------------------------------------------------------------------------
// Returns the array of image data required for gdk_pixbuf_new_from_inline.
static const guint8* GetTilesetImageData(DungeonTileset tileset)
{
    switch (tileset)
    {
    case TILESET_CAVE:
        return tilesetDungeonCave;
    default:
        return NULL;
    }
}

// ------------------------------------------------------------------------------------------------
// Returns the tile image for a TERRAIN_WALL cell based on the surrounding cells.
static DungeonTile GetWallTile(Point *position)
{
    guint tile = TILE_NULL;
    Terrain neighbors[DIR_COUNT] = {TERRAIN_NULL};

    // Get terrain for each neighboring cell.
    for (guint i = 0; i < DIR_COUNT; i++)
    {
        Point cellNeighbor = {position->x + hMovement[i], position->y + vMovement[i]};

        neighbors[i] = GetCellTerrain(&cellNeighbor);
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

    return (DungeonTile)tile;
}

// ------------------------------------------------------------------------------------------------
// Returns the GdkPixbuf from the tiles array for the given actor.
static GdkPixbuf* GetTileForActor(Actor *actor)
{
    ActorSpecies species = actor->species;
    DungeonTile tile;

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
static GdkPixbuf* GetTileForTerrain(Point *position)
{
    Terrain terrain = GetCellTerrain(position);
    DungeonTile tile;

    switch (terrain)
    {
    case TERRAIN_FLOOR:
        tile = TILE_FLOOR;
        break;
    case TERRAIN_WALL:
        tile = GetWallTile(position);
        break;
    default:
        tile = TILE_NULL;
    }

    return GetPixbufFromTile(tile);
}

// ------------------------------------------------------------------------------------------------
// Returns a GdkPixbuf based on the given idex to the tiles array.
static GdkPixbuf* GetPixbufFromTile(DungeonTile tile)
{
    return dungeonTiles[tile];
}

// ------------------------------------------------------------------------------------------------
// Read image data for the given tileset into the GdkPixbufs tiles array.
void LoadDungeonTiles(DungeonTileset tileset)
{
    GdkPixbuf *source = NULL;
    GError * error = NULL;

    source = gdk_pixbuf_new_from_inline(-1, GetTilesetImageData(tileset), FALSE, &error);

    for (guint i = 0; i < TILE_COUNT; i++)
    {
        guint pixelX = (i % TILESET_WIDTH) * TILE_SIZE_16;
        guint pixelY = (i / TILESET_WIDTH) * TILE_SIZE_16;

        dungeonTiles[i] = gdk_pixbuf_new_subpixbuf(source, pixelX, pixelY, TILE_SIZE_16, TILE_SIZE_16);
    }

    g_object_unref(source);
}

// ------------------------------------------------------------------------------------------------
// Scale the GdkPixbuf tiles based on the viewPort's zoomLevel.
void ScaleTileForZoom(void)
{
    guint tileSize = GetTileSizeForZoomLevel(GetViewPortZoomLevel());
    for (guint i = 0; i < TILE_COUNT; i++)
    {
        dungeonTiles[i] = gdk_pixbuf_scale_simple(dungeonTiles[i], tileSize, tileSize, GDK_INTERP_NEAREST);
    }
}

// ------------------------------------------------------------------------------------------------
// Free the GdkPixbufs for the tiles array.
void FreeDungeonTiles(void)
{
    // Free memory used by GdkPixbufs.
    for (guint i = 0; i < TILE_COUNT; i++)
    {
        g_object_unref(dungeonTiles[i]);
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
    if (level < ZOOM_LEVEL_OFF)
        zoomLevel = ZOOM_LEVEL_OFF;
    else if (level > ZOOM_LEVEL_PEAK)
        zoomLevel = ZOOM_LEVEL_PEAK;
    else
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
                Point pixel = {tileSize * x, tileSize * y};

                // Zoom levels other than ZOOM_LEVEL_PEAK have an even number of tiles to draw,
                // so we draw them offset by half a tile to keep the player centered.
                if (zoomLevel != ZOOM_LEVEL_PEAK)
                {
                    pixel.x -= tileSize / 2;
                    pixel.y -= tileSize / 2;
                }

                // The dungeon cell to be drawn in the viewPort.
                Point cell = {viewPosition->x + x, viewPosition->y + y};

                // Draws the terrain for the cell.
                gdk_cairo_set_source_pixbuf(context, GetTileForTerrain(&cell), pixel.x, pixel.y);
                cairo_paint(context);

                // If position contains an actor, draw it over the terrain.
                if (GetCellsActor(&cell) != NULL)
                {
                    gdk_cairo_set_source_pixbuf(context, dungeonTiles[TILE_AT], pixel.x, pixel.y);
                    cairo_paint(context);
                }

                // If position is also the selected cell, draw the cursor over everything else.
                if (selectedCell->x == cell.x && selectedCell->y == cell.y)
                {
                    gdk_cairo_set_source_pixbuf(context, dungeonTiles[TILE_CELL_SELECTED], pixel.x, pixel.y);
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
// Callback function for when the viewPort is initially pressed.
static gboolean on_viewPort_click_press(GtkWidget *widget, GdkEventButton *event, gpointer userData)
{
    Point pixelPos = {(gint)event->x, (gint)event->y};

    SetGestureStartPos(&pixelPos);
    SetGestureStartTime();

    return FALSE;
}

// ------------------------------------------------------------------------------------------------
// Callback function for when the click on the viewPort is released.
static gboolean on_viewPort_click_release(GtkWidget *widget, GdkEventButton *event, gpointer userData)
{
    Point currentPos = {(gint)event->x, (gint)event->y};
    SetGestureEndPos(&currentPos);
    SetGestureEndTime();
    GestureType gesture = GetGestureType();

    guint zoomLevel = GetViewPortZoomLevel();
    gint tileSize = GetTileSizeForZoomLevel((ZoomLevel)zoomLevel);
    Point pixelPos = {(gint)event->x, (gint)event->y};
    Point clickedTile = {0};
    Point *viewPosition = GetViewPosition();
    Point *oldSelectedCell = GetSelectedCell();
    Point newSelectedCell = {0};

    if (gesture == GESTURE_SINGLE_TAP)
    {
        // Zoom levels other than ZOOM_LEVEL_PEAK have an even number of tiles to draw,
        // so we draw them offset by half a tile to keep the player centered.
        if (zoomLevel == ZOOM_LEVEL_PEAK)
        {
            clickedTile.x = pixelPos.x / tileSize;
            clickedTile.y = pixelPos.y / tileSize;
        }
        else
        {
            clickedTile.x = (pixelPos.x + tileSize / 2) / tileSize;
            clickedTile.y = (pixelPos.y + tileSize / 2) / tileSize;
        }

        // Get the dungeon cell of the clicked tile.
        newSelectedCell.x = viewPosition->x + clickedTile.x;
        newSelectedCell.y = viewPosition->y + clickedTile.y;

        // Center viewPort on selected dungeon cell if it was clicked again and is traversable.
        if (newSelectedCell.x == oldSelectedCell->x && newSelectedCell.y == oldSelectedCell->y)
        {
            if (IsTerrainTraversable(&newSelectedCell))
            {
                CenterViewPortOn(&newSelectedCell);
                SetActorPosition(&actors[0], &newSelectedCell);
            }
        }
        else
        {
            SetSelectedCell(&newSelectedCell);
        }
    }
    else if (gesture == GESTURE_SWIPE)
    {
        Direction swipeDirection = GetSwipeDirection();
        Actor *player = GetActor(0);

        if (IsCardinalDirection(swipeDirection))
        {
            // Move in the opposite direction, as the player's position on screen is fixed;
            // it is the map underneath them that is moving.
            // TODO: global variable setting for not using the opposite direction of the swipe.
            ActionWalk(player, GetOppositeDirection(GetSwipeDirection()));
            CenterViewPortOn(&player->position);
        }
    }

    // Queue update to the viewPort.
    gtk_widget_queue_draw(GTK_WIDGET(viewPort));

    return FALSE;
}
