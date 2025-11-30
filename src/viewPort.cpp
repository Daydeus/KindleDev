#include <gtk-2.0/gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <glib-2.0/glib.h>
#include <cairo/cairo.h>
#include <cstdlib>
#include "global.h"
#include "touchGesture.h"
#include "actor.h"
#include "dungeonCell.h"
#include "menuBox.h"
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
Point selectedCell = {0}; // The current player-selected dungeonCell in the viewPort.
gboolean zoomIsOn = TRUE;
ViewPortMode viewPortMode = MODE_CHARACTER;

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

static void DrawDungeon(cairo_t *context);
static void DrawViewPortBorders(cairo_t *context);
static gboolean on_viewPort_update(GtkWidget *widget, cairo_t *context, gpointer userData);
static void DoViewPortInputCharacter(Point *inputPos);
static void DoViewPortInputSelector(Point *inputPos);
static gboolean on_viewPort_click_press(GtkWidget *widget, GdkEventButton *event, gpointer userData);
static gboolean on_viewPort_click_release(GtkWidget *widget, GdkEventButton *event, gpointer userData);

// ------------------------------------------------------------------------------------------------
// Load GdkPixbuf tiles and initialize the dungeon viewPort.
void InitViewPort(void)
{
    LoadDungeonTiles(TILESET_CAVE);
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
// Gets the dungeonCell position of the currently selected cell in the viewPort.
Point* GetSelectedCell(void)
{
    return &selectedCell;
}

// ------------------------------------------------------------------------------------------------
// Sets the currently selected dungeonCell in the viewPort to the given position.
void SetSelectedCell(Point *position)
{
    position->x = ClampValue(position->x, 0, DUNGEON_WIDTH);
    position->y = ClampValue(position->y, 0, DUNGEON_HEIGHT);

    selectedCell = *position;
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
// Gets the viewPortMode (which determines if input is for the player or selectedCell).
ViewPortMode GetViewPortMode(void)
{
    return viewPortMode;
}

// ------------------------------------------------------------------------------------------------
// Sets the viewPortMode (which determines if input is for the player or selectedCell).
void SetViewPortMode(ViewPortMode mode)
{
    viewPortMode = mode;
}

// ------------------------------------------------------------------------------------------------
// Draw the tiles of the visible dungeonCells and actors.
static void DrawDungeon(cairo_t *context)
{
    gboolean zoomIsOn = GetViewPortZoom();
    gint tileSize = GetTileSizeForZoom(zoomIsOn);
    Point *viewPosition = GetViewPosition();
    Point *selectedCell = GetSelectedCell();

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
            if (GetCellsActor(&cell) != NULL)
            {
                gdk_cairo_set_source_pixbuf(context, dungeonTiles[TILE_AT], pixel.x, pixel.y);
                cairo_paint(context);
            }

            // If viewPortMode is MODE_SELECTOR and the position is also the selected cell, draw
            // the cursor over everything else.
            if (GetViewPortMode() == MODE_SELECTOR && selectedCell->x == cell.x && selectedCell->y == cell.y)
            {
                gdk_cairo_set_source_pixbuf(context, dungeonTiles[TILE_CELL_SELECTED], pixel.x, pixel.y);
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
static gboolean on_viewPort_update(GtkWidget *widget, cairo_t *context, gpointer userData)
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
// Process input for the viewPort when it is in MODE_CHARACTER. The viewPort will be centered on the
// player character and input will only affect the player.
static void DoViewPortInputCharacter(Point *inputPos)
{
    GestureType gesture = GetGestureType();

    if (gesture == GESTURE_SWIPE)
    {
        Direction swipeDirection = GetSwipeDirection();
        Actor *player = GetActor(0);

        if (IsCardinalDirection(swipeDirection))
        {
            // Move in the opposite direction, as the player's position on screen is fixed;
            // it is the map underneath them that is moving.
            // TODO: global variable setting for not using the opposite direction of the swipe.
            ActionWalk(player, GetOppositeDirection(swipeDirection));
            CenterViewPortOn(&player->position);
        }
        else if (swipeDirection == DIR_NORTH_WEST)
        {
            SetViewPortMode(MODE_SELECTOR);
            SetSelectedCell(&player->position);
        }
        else if (swipeDirection == DIR_SOUTH_WEST)
        {
            gboolean zoomIsOn = GetViewPortZoom();

            zoomIsOn = !zoomIsOn;
            SetViewPortZoom(zoomIsOn);
            ScaleTileForZoom(zoomIsOn);
            CenterViewPortOn(&player->position);

            gtk_widget_queue_draw(GTK_WIDGET(viewPort));
            gtk_widget_queue_draw(GTK_WIDGET(menuBox));
        }
    }
}

// ------------------------------------------------------------------------------------------------
// Process input for the viewPort when it is in MODE_SELECTOR. The viewPort will be centered on the
// selectedCell and all movement will update it instead of the player.
static void DoViewPortInputSelector(Point *inputPos)
{
    GestureType gesture = GetGestureType();
    gboolean zoomIsOn = GetViewPortZoom();
    gint tileSize = GetTileSizeForZoom(zoomIsOn);
    Point *viewPosition = GetViewPosition();
    Point clickedTile = {0};
    Point newSelectedCell = {0};

    // When the viewPort's length has an even number of tiles, the tiles are drawn offset by half
    // a tile to keep the player centered on-screen.
    if (gesture == GESTURE_SINGLE_TAP)
    {
        if (IsValueEven(VIEWPORT_WIDTH / tileSize))
            clickedTile.x = (inputPos->x + tileSize / 2) / tileSize;
        else
            clickedTile.x = inputPos->x / tileSize;
        if (IsValueEven(VIEWPORT_HEIGHT / tileSize))
            clickedTile.y = (inputPos->y + tileSize / 2) / tileSize;
        else
            clickedTile.y = inputPos->y / tileSize;

        // Get the dungeon cell of the clicked tile.
        newSelectedCell.x = viewPosition->x + clickedTile.x;
        newSelectedCell.y = viewPosition->y + clickedTile.y;

        SetSelectedCell(&newSelectedCell);
        CenterViewPortOn(&newSelectedCell);

    }
    else if (gesture == GESTURE_SWIPE)
    {
        Direction swipeDirection = GetSwipeDirection();

        if (IsCardinalDirection(swipeDirection))
        {
            // Move in the opposite direction, as the player's position on screen is fixed;
            // it is the map underneath them that is moving.
            // TODO: global variable setting for not using the opposite direction of the swipe.
            Point *oldSelectedCell = GetSelectedCell();

            newSelectedCell.x = oldSelectedCell->x + hMovement[GetOppositeDirection(swipeDirection)];
            newSelectedCell.y = oldSelectedCell->y + vMovement[GetOppositeDirection(swipeDirection)];

            SetSelectedCell(&newSelectedCell);
            CenterViewPortOn(&newSelectedCell);
        }
        else if (swipeDirection == DIR_NORTH_WEST)
        {
            Actor *player = GetActor(0);

            SetViewPortMode(MODE_CHARACTER);
            CenterViewPortOn(&player->position);
        }
        else if (swipeDirection == DIR_SOUTH_WEST)
        {
            gboolean zoomIsOn = GetViewPortZoom();

            zoomIsOn = !zoomIsOn;
            SetViewPortZoom(zoomIsOn);
            ScaleTileForZoom(zoomIsOn);
            CenterViewPortOn(GetSelectedCell());

            gtk_widget_queue_draw(GTK_WIDGET(viewPort));
            gtk_widget_queue_draw(GTK_WIDGET(menuBox));
        }
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
    Point inputPos = {(gint)event->x, (gint)event->y};
    SetGestureEndPos(&inputPos);
    SetGestureEndTime();

    if (GetViewPortMode() == MODE_CHARACTER)
        DoViewPortInputCharacter(&inputPos);
    else
        DoViewPortInputSelector(&inputPos);

    // Queue update to the viewPort.
    gtk_widget_queue_draw(GTK_WIDGET(viewPort));

    return FALSE;
}
