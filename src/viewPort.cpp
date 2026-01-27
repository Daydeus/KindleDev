#include <gtk-2.0/gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <glib-2.0/glib.h>
#include <cairo/cairo.h>
#include <cstdlib>
#include "global.h"
#include "touchGesture.h"
#include "action.h"
#include "actor.h"
#include "dungeonCell.h"
#include "dungeonMaster.h"
#include "menuBox.h"
#include "pathfinding.h"
#include "tile.h"
#include "viewPort.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

GtkDrawingArea *viewPort = NULL;
Point viewPosition = {0}; // The dungeonCell position of the viewPort origin.
gboolean zoomIsOn = TRUE;

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

static void DrawDungeon(cairo_t *context);
static void DrawViewPortBorders(cairo_t *context);
static gboolean on_viewPort_update(GtkWidget *widget, gpointer userData);
static void DoViewPortInput(Point *inputPos);
static gboolean on_viewPort_click_press(GtkWidget *widget, GdkEventButton *event, gpointer userData);
static gboolean on_viewPort_click_release(GtkWidget *widget, GdkEventButton *event, gpointer userData);

// ------------------------------------------------------------------------------------------------
// Load GdkPixbuf tiles and initialize the dungeon viewPort.
void InitViewPort(void)
{
    LoadDungeonTiles(TILESET_CAVE);
    LoadActorTiles();
    ScaleTileForZoom(GetViewPortZoom());

    // Initialize the viewPort.
    viewPort = GTK_DRAWING_AREA(gtk_drawing_area_new());
    gtk_widget_set_size_request(GTK_WIDGET(viewPort), VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
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
    guint tileSize = GetTileSizeForZoom(GetViewPortZoom());
    gint viewPortHalfWidth = VIEWPORT_WIDTH / tileSize / 2;
    gint viewPortHalfHeight = VIEWPORT_HEIGHT / tileSize / 2;
    Point newPosition = {position->x - viewPortHalfWidth, position->y - viewPortHalfHeight};

    SetViewPosition(&newPosition);
}

// ------------------------------------------------------------------------------------------------
// Get whether zoom is active on the viewPort.
gboolean GetViewPortZoom(void)
{
    return zoomIsOn;
}

// ------------------------------------------------------------------------------------------------
// Set whether zoom is active on the viewPort.
void SetViewPortZoom(gboolean boolean)
{
    zoomIsOn = boolean;
}

// ------------------------------------------------------------------------------------------------
// Returns if the given position is visible in the viewPort.
gboolean IsPositionOnScreen(Point *position)
{
    // Temporary function until Fog of War and sightId is implemented for cells.
    // TODO: remove
    gint tileSize = GetTileSizeForZoom(zoomIsOn);
    gint viewPortWidth = VIEWPORT_WIDTH / tileSize;
    gint viewPortHeight = VIEWPORT_HEIGHT / tileSize;

    if (IsWithinRectangle(position, GetViewPosition(), viewPortWidth, viewPortHeight))
        return TRUE;
    else
        return FALSE;
}

// ------------------------------------------------------------------------------------------------
// Draw the tiles of the visible dungeonCells and actors.
static void DrawDungeon(cairo_t *context)
{
    gboolean zoomIsOn = GetViewPortZoom();
    gint tileSize = GetTileSizeForZoom(zoomIsOn);
    Point *viewPosition = GetViewPosition();

    for (gint y = 0; y <= VIEWPORT_HEIGHT / tileSize; y++)
    {
        for (gint x = 0; x <= VIEWPORT_WIDTH / tileSize; x++)
        {
            // The pixel position within the viewPort to be changed.
            Point pixel = {tileSize * x, tileSize * y};

            // When the viewPort's length has an even number of tiles, the tiles are drawn
            // offset by half a tile to keep the player centered on-screen.
            if (IsValueEven(VIEWPORT_WIDTH / tileSize))
                pixel.x -= tileSize / 2;
            if (IsValueEven(VIEWPORT_HEIGHT / tileSize))
                pixel.y -= tileSize / 2;

            // The dungeon cell to be drawn in the viewPort.
            Point cell = {viewPosition->x + x, viewPosition->y + y};

            // Draws the terrain for the cell.
            gdk_cairo_set_source_pixbuf(context, GetTileForTerrain(&cell), pixel.x, pixel.y);
            cairo_paint(context);

            // If position contains an actor, draw it over the terrain.
            Actor *actorToDraw = GetCellsActor(&cell);
            if (actorToDraw != NULL)
            {
                gdk_cairo_set_source_pixbuf(context, GetTileForActor(actorToDraw), pixel.x, pixel.y);
                cairo_paint(context);
            }

            // Draw the cell selector icon if the current cell is selected and selectedCell
            // status is not off.
            if (GetSelectedCellStatus() != STATUS_OFF && IsSamePoint(&cell, GetSelectedCell()))
            {
                gdk_cairo_set_source_pixbuf(context, GetTileForCellSelector(), pixel.x, pixel.y);
                cairo_paint(context);
            }
        }
    }
}

// ------------------------------------------------------------------------------------------------
// Draw the border for the MenuBox.
static void DrawViewPortBorders(cairo_t *context)
{
    // ViewPort edges for the North, East, South, and West directions.
    #define EDGE_N 0
    #define EDGE_E (VIEWPORT_WIDTH - TILE_SIZE_MB)
    #define EDGE_S (VIEWPORT_HEIGHT - TILE_SIZE_MB)
    #define EDGE_W 0

    guint tileVariant = 0;

    // Draw the vertical borders.
    for (guint i = TILE_SIZE_MB; i < VIEWPORT_WIDTH - TILE_SIZE_MB; i += TILE_SIZE_MB)
    {
        tileVariant = (i / TILE_SIZE_MB) % 3;
        gdk_cairo_set_source_pixbuf(context, borderTiles[TILE_BORDER_NORTH_1 + tileVariant], i, EDGE_N);
        cairo_paint(context);

        gdk_cairo_set_source_pixbuf(context, borderTiles[TILE_BORDER_SOUTH_1 + tileVariant], i, EDGE_S);
        cairo_paint(context);
    }

    // Draw the horizontal borders.
    for (guint i = TILE_SIZE_MB; i < VIEWPORT_HEIGHT - TILE_SIZE_MB; i += TILE_SIZE_MB)
    {
        tileVariant = (i / TILE_SIZE_MB) % 3;
        gdk_cairo_set_source_pixbuf(context, borderTiles[TILE_BORDER_EAST_1 + tileVariant], EDGE_E, i);
        cairo_paint(context);

        gdk_cairo_set_source_pixbuf(context, borderTiles[TILE_BORDER_WEST_1 + tileVariant], EDGE_W, i);
        cairo_paint(context);
    }

    // Draw the corners.
    gdk_cairo_set_source_pixbuf(context, borderTiles[TILE_BORDER_CORNER_NORTH_EAST], EDGE_E, EDGE_N);
    cairo_paint(context);
    gdk_cairo_set_source_pixbuf(context, borderTiles[TILE_BORDER_CORNER_SOUTH_EAST], EDGE_E, EDGE_S);
    cairo_paint(context);
    gdk_cairo_set_source_pixbuf(context, borderTiles[TILE_BORDER_CORNER_SOUTH_WEST], EDGE_W, EDGE_S);
    cairo_paint(context);
    gdk_cairo_set_source_pixbuf(context, borderTiles[TILE_BORDER_CORNER_NORTH_WEST], EDGE_W, EDGE_N);
    cairo_paint(context);

    #undef EDGE_N
    #undef EDGE_E
    #undef EDGE_S
    #undef EDGE_W
}

// ------------------------------------------------------------------------------------------------
// Callback function to update the tiles shown on the viewPort.
static gboolean on_viewPort_update(GtkWidget *widget, gpointer userData)
{
    // Get the GdkWindow from the widget
    GdkWindow *window = gtk_widget_get_window(widget);

    if (window)
    {
        // Create a Cairo context from the GdkWindow
        cairo_t *context = gdk_cairo_create(window);

        DrawDungeon(context);
        DrawViewPortBorders(context);

        // Clean up the Cairo context
        cairo_destroy(context);
    }
    return FALSE;
}

// ------------------------------------------------------------------------------------------------
// Process input for the viewPort. The viewPort will be centered on the player character.
static void DoViewPortInput(Point *inputPos)
{
    GestureType gesture = GetGestureType();
    gboolean zoomIsOn = GetViewPortZoom();
    gint tileSize = GetTileSizeForZoom(zoomIsOn);
    Actor *player = GetActor(PLAYER_ACTOR_INDEX);

    if (gesture == GESTURE_SINGLE_TAP)
    {
        Point viewPosition = *GetViewPosition();
        Point tappedTile = {inputPos->x / tileSize, inputPos->y / tileSize};
        Point tappedCell = {0};

        // When the viewPort's length in tiles is even, the tiles are drawn offset by half a
        // tile to keep the player centered on-screen. This must be taken into account when
        // determining which tile was tapped.
        if (IsValueEven(VIEWPORT_WIDTH / tileSize))
            tappedTile.x = (inputPos->x + tileSize / 2) / tileSize;
        if (IsValueEven(VIEWPORT_HEIGHT / tileSize))
            tappedTile.y = (inputPos->y + tileSize / 2) / tileSize;

        // Get the dungeonCell of the clicked tile.
        tappedCell.x = viewPosition.x + tappedTile.x;
        tappedCell.y = viewPosition.y + tappedTile.y;

        // If the player is tapped and zoom is on, turn zoom off.
        // If the screen is tapped and zoom is off, turn zoom on.
        if (IsSamePoint(&tappedCell, GetActorPosition(player)))
        {
            zoomIsOn = !zoomIsOn;
            SetViewPortZoom(zoomIsOn);
            ScaleTileForZoom(zoomIsOn);
            CenterViewPortOn(&player->position);

            gtk_widget_queue_draw(GTK_WIDGET(viewPort));
            if (GetMenuState() == STATE_SETTINGS)
                gtk_widget_queue_draw(GTK_WIDGET(menuBox));
        }
        else
        {
            Point *selectedCell = GetSelectedCell();

            // If selectedCell is tapped again, auto-navigate player to the dungeoncell if possible.
            if (IsSamePoint(&tappedCell, selectedCell) && IsTerrainTraversable(&tappedCell)
                && DoesPathToCellExist(selectedCell))
            {
                SetSelectedCellStatus(STATUS_LOCKED);
                SetActionForPlayer(ACTION_WALK_AUTO);
                g_timeout_add(500, (GSourceFunc)ProcessTurn, NULL);
            }
            else
            {
                SetSelectedCellStatus(STATUS_UNLOCKED);
                SetSelectedCell(&tappedCell);

                SetActionForPlayer(ACTION_NONE);
            }

            gtk_widget_queue_draw(GTK_WIDGET(viewPort));
        }
    }
    else if (gesture == GESTURE_SWIPE)
    {
        if (GetSelectedCellStatus() != STATUS_LOCKED)
        {
            Direction swipeDirection = GetOppositeDirection(GetSwipeDirection());

            SetActionForPlayer(GetWalkFromDirection(swipeDirection));
            ProcessTurn(NULL);
        }
        else
        {
            SetSelectedCellStatus(STATUS_UNLOCKED);
            SetActionForPlayer(ACTION_NONE);
        }

        gtk_widget_queue_draw(GTK_WIDGET(viewPort));
    }
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
    if (GetInputBlockStatus() == INPUT_IS_BLOCKED)
        return FALSE;

    Point inputPos = {(gint)event->x, (gint)event->y};
    SetGestureEndPos(&inputPos);
    SetGestureEndTime();

    DoViewPortInput(&inputPos);

    return FALSE;
}
