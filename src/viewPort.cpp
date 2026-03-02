#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include <cairo/cairo.h>
#include <cstdlib>
#include "action.h"
#include "actor.h"
#include "dungeonCell.h"
#include "dungeonMaster.h"
#include "fieldOfView.h"
#include "global.h"
#include "menu.h"
#include "pathfinding.h"
#include "tile.h"
#include "touchGesture.h"
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
    LoadTerrainTiles();
    LoadActorTiles();

    // Initialize the viewPort.
    viewPort = GTK_DRAWING_AREA(gtk_drawing_area_new());
    gtk_widget_set_size_request(GTK_WIDGET(viewPort), VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
    SetWidgetBgColor(GTK_WIDGET(viewPort), COLOR_BLACK);

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
    gint viewPortHalfWidth = VIEWPORT_WIDTH / TILE_SIZE_VP / 2;
    gint viewPortHalfHeight = VIEWPORT_HEIGHT / TILE_SIZE_VP / 2;
    Point newPosition = {position->x - viewPortHalfWidth, position->y - viewPortHalfHeight};

    SetViewPosition(&newPosition);
}

// ------------------------------------------------------------------------------------------------
// Returns if the given position is present in the viewPort.
gboolean IsPositionOnScreen(Point *position)
{
    gint viewPortWidth = VIEWPORT_WIDTH / TILE_SIZE_VP;
    gint viewPortHeight = VIEWPORT_HEIGHT / TILE_SIZE_VP;

    if (IsWithinRectangle(position, GetViewPosition(), viewPortWidth, viewPortHeight))
        return TRUE;
    else
        return FALSE;
}

// ------------------------------------------------------------------------------------------------
// Draw the tiles of the visible dungeonCells and actors.
static void DrawDungeon(cairo_t *context)
{
    Point *viewPosition = GetViewPosition();

    for (gint y = 0; y <= VIEWPORT_HEIGHT / TILE_SIZE_VP; y++)
    {
        for (gint x = 0; x <= VIEWPORT_WIDTH / TILE_SIZE_VP; x++)
        {
            // The pixel position within the viewPort to be changed.
            Point pixel = {x * TILE_SIZE_VP, y * TILE_SIZE_VP};

            // The dungeon cell to be drawn in the viewPort.
            Point cell = {viewPosition->x + x, viewPosition->y + y};

            // Only draw cell terrain and cell selector if the player has seen the cell before.
            if (GetCellSightId(&cell) != CELL_UNEXPLORED || GetFogOfWarStatus() == FALSE)
            {
                // Draws the terrain for the cell.
                gdk_cairo_set_source_pixbuf(context, GetTileForTerrain(&cell), pixel.x, pixel.y);
                cairo_paint(context);

                // Only draw actors if player has line of sight.
                if (IsVisibleToPlayer(&cell))
                {
                    // If position contains an non-dead actor, draw it over the terrain.
                    Actor *actorToDraw = GetCellsActor(&cell);
                    if (actorToDraw != NULL && !IsActorDead(actorToDraw))
                    {
                        gdk_cairo_set_source_pixbuf(context, GetTileForActor(actorToDraw), pixel.x, pixel.y);
                        cairo_paint(context);

                        if (HasActorBeenDamaged(actorToDraw))
                        {
                            gdk_cairo_set_source_pixbuf(context, actorTiles[TILE_ACTOR_DAMAGE_INDICATOR], pixel.x, pixel.y);
                            cairo_paint(context);
                            SetActorDamaged(actorToDraw, FALSE);
                        }
                    }
                }

                // Draw the cell selector icon on the current cell if it is selected.
                if (GetSelectedCellStatus() != STATUS_OFF && IsSamePoint(&cell, GetSelectedCell()))
                {
                    gdk_cairo_set_source_pixbuf(context, GetTileForCellSelector(), pixel.x, pixel.y);
                    cairo_paint(context);
                }
            }
        }
    }
}

// ------------------------------------------------------------------------------------------------
// Draw the border for the viewPort.
static void DrawViewPortBorders(cairo_t *context)
{
    // ViewPort edges for the North, East, South, and West directions.
    #define EDGE_N 0
    #define EDGE_E (VIEWPORT_WIDTH - TILE_SIZE_BORDER)
    #define EDGE_S (VIEWPORT_HEIGHT - TILE_SIZE_BORDER)
    #define EDGE_W 0

    guint tileVariant = 0;

    // Draw the vertical borders.
    for (guint i = TILE_SIZE_BORDER; i < VIEWPORT_WIDTH - TILE_SIZE_BORDER; i += TILE_SIZE_BORDER)
    {
        tileVariant = (i / TILE_SIZE_BORDER) % 3;
        gdk_cairo_set_source_pixbuf(context, borderTiles[TILE_BORDER_NORTH_1 + tileVariant], i, EDGE_N);
        cairo_paint(context);

        gdk_cairo_set_source_pixbuf(context, borderTiles[TILE_BORDER_SOUTH_1 + tileVariant], i, EDGE_S);
        cairo_paint(context);
    }

    // Draw the horizontal borders.
    for (guint i = TILE_SIZE_BORDER; i < VIEWPORT_HEIGHT - TILE_SIZE_BORDER; i += TILE_SIZE_BORDER)
    {
        tileVariant = (i / TILE_SIZE_BORDER) % 3;
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
    Actor *player = GetActor(PLAYER_ACTOR_INDEX);
    Point viewPosition = *GetViewPosition();
    Point tappedTile = {inputPos->x / TILE_SIZE_VP, inputPos->y / TILE_SIZE_VP};
    Point tappedCell = {0};
    Point *selectedCell = GetSelectedCell();

    // Get the dungeonCell of the clicked tile.
    tappedCell.x = viewPosition.x + tappedTile.x;
    tappedCell.y = viewPosition.y + tappedTile.y;

    if (gesture == GESTURE_SINGLE_TAP)
    {
        Point *playerPos = GetActorPosition(player);

        // If selectedCell is tapped again, auto-navigate player to the dungeoncell if possible.
        if (IsSamePoint(&tappedCell, selectedCell) && !IsSamePoint(selectedCell, playerPos)
            && IsTerrainTraversable(&tappedCell) && DoesPathToCellExist(selectedCell)
            && (GetCellSightId(&tappedCell) != CELL_UNEXPLORED || GetFogOfWarStatus() == FALSE))
        {
            guint distance = GetDistanceBetween(GetActorPosition(player), selectedCell);

            if (distance == ATTACK_DISTANCE && IsCellOccupiedByActor(selectedCell))
            {
                Direction direction = GetTravelDirectionBetweenCells(playerPos, selectedCell);
                SetActionForPlayer(GetAttackFromDirection(direction));
                ProcessTurn(NULL);
            }
            else
            {
                SetSelectedCellStatus(STATUS_LOCKED);
                SetActionForPlayer(ACTION_WALK_AUTO);
                g_timeout_add(TURN_TIMER_AUTO, (GSourceFunc)ProcessTurn, NULL);
            }
        }
        else
        {
            // Only change selected cell if it is not locked and the cell has been explored.
            if (GetSelectedCellStatus() == STATUS_LOCKED)
            {
                SetSelectedCellStatus(STATUS_UNLOCKED);
                SetActionForPlayer(ACTION_NONE);
            }
            else if (GetSelectedCellStatus() != STATUS_LOCKED
                && (GetCellSightId(&tappedCell) != CELL_UNEXPLORED || GetFogOfWarStatus() == FALSE))
            {
                SetSelectedCellStatus(STATUS_UNLOCKED);
                SetSelectedCell(&tappedCell);

                // Make player face the selectedCell.
                gint facing = playerPos->x - selectedCell->x;
                if (facing > 0)
                    UpdateActorFacing(player, DIR_WEST);
                else if (facing < 0)
                    UpdateActorFacing(player, DIR_EAST);
            }
        }
    }
    else if (gesture == GESTURE_HOLD_TAP)
    {
        // Placeholder
    }
    else if (gesture == GESTURE_SWIPE)
    {
        if (GetSelectedCellStatus() != STATUS_LOCKED)
        {
            Direction swipeDirection = GetOppositeDirection(GetSwipeDirection());
            Point newPosition = *selectedCell;

            newPosition.x += hMovement[swipeDirection];
            newPosition.y += vMovement[swipeDirection];

            if (GetCellSightId(&newPosition) != CELL_UNEXPLORED
                || (GetFogOfWarStatus() == FALSE && !IsOutsideDungeon(&newPosition)))
                SetSelectedCell(&newPosition);
        }
        else
        {
            SetSelectedCellStatus(STATUS_UNLOCKED);
            SetActionForPlayer(ACTION_NONE);
        }
    }

    gtk_widget_queue_draw(GTK_WIDGET(viewPort));
    if (GetMenuState() == STATE_MINIMAP)
        gtk_widget_queue_draw(GTK_WIDGET(menu));
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

    DoViewPortInput(&inputPos);

    return FALSE;
}
