#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include "global.h"
#include "tile.h"
#include "actor.h"
#include "menuBox.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

GtkDrawingArea *menuBox = NULL;

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

static void DrawMovementArrows(cairo_t *context);
static void DrawMenuBoxBorders(cairo_t *context);
static Direction WasMovementArrowClicked(Point *position);

// ------------------------------------------------------------------------------------------------
// Load GdkPixbuf tiles and initialize the menuBbox for the player.
void InitMenuBox(void)
{
    LoadMenuBoxTiles();

    // Initialize menuBox.
    menuBox = GTK_DRAWING_AREA(gtk_drawing_area_new());
    gtk_widget_set_size_request(GTK_WIDGET(menuBox), MENU_BOX_WIDTH, MENU_BOX_HEIGHT);
    SetWidgetBgColor(GTK_WIDGET(menuBox), COLOR_BLACK);

    // Set up signals.
    g_signal_connect(menuBox, "expose_event", G_CALLBACK(on_menuBox_update), NULL);
    g_signal_connect(menuBox, "button_press_event", G_CALLBACK(on_menuBox_click), NULL);
    gtk_widget_set_events(GTK_WIDGET(menuBox), GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK);
}

// ------------------------------------------------------------------------------------------------
// Draw the border for the MenuBox.
static void DrawMenuBoxBorders(cairo_t *context)
{
    // MenuBox edges for the North, East, South, and West directions.
    #define EDGE_N 0
    #define EDGE_E (MENU_BOX_WIDTH - TILE_SIZE_MB)
    #define EDGE_S (MENU_BOX_HEIGHT - TILE_SIZE_MB)
    #define EDGE_W 0

    guint tileVariant = 0;

    // Draw the vertical borders.
    for (guint i = TILE_SIZE_MB; i < MENU_BOX_WIDTH - TILE_SIZE_MB; i += TILE_SIZE_MB)
    {
        tileVariant = (i / TILE_SIZE_MB) % 3;
        gdk_cairo_set_source_pixbuf(context, borderTiles[TILE_BORDER_NORTH_1 + tileVariant], i, EDGE_N);
        cairo_paint(context);

        gdk_cairo_set_source_pixbuf(context, borderTiles[TILE_BORDER_SOUTH_1 + tileVariant], i, EDGE_S);
        cairo_paint(context);
    }

    // Draw the horizontal borders.
    for (guint i = TILE_SIZE_MB; i < MENU_BOX_HEIGHT - TILE_SIZE_MB; i += TILE_SIZE_MB)
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
// Draw the arrow tiles for controlling the player.
static void DrawMovementArrows(cairo_t *context)
{
    // Loop through DIR_NORTH, DIR_EAST, DIR_SOUTH, and DIR_WEST.
    for (gint i = 0; i < DIR_CARDINAL_COUNT; i++)
    {
        Point pixelPos = {ARROWS_CENTER_X, ARROWS_CENTER_Y};
        pixelPos.x += hMovement[i] * TILE_SIZE_MB;
        pixelPos.y += vMovement[i] * TILE_SIZE_MB;

        gdk_cairo_set_source_pixbuf(context, menuBoxTiles[TILE_ARROW_NORTH + i], pixelPos.x, pixelPos.y);
        cairo_paint(context);
    }
}

// ------------------------------------------------------------------------------------------------
// Returns the movement direction associated with an arrow tile if one was clicked. Otherwise, it
// returns DIR_ALL_COUNT;
static Direction WasMovementArrowClicked(Point *position)
{
    // Loop through DIR_NORTH, DIR_EAST, DIR_SOUTH, and DIR_WEST.
    for (gint i = 0; i < DIR_CARDINAL_COUNT; i++)
    {
        Point arrowTile = {ARROWS_CENTER_X, ARROWS_CENTER_Y};
        arrowTile.x += hMovement[i] * TILE_SIZE_MB;
        arrowTile.y += vMovement[i] * TILE_SIZE_MB;

        if (IsWithinRectangle(position, &arrowTile, TILE_SIZE_MB, TILE_SIZE_MB))
            return (Direction)i;
    }

    return DIR_ALL_COUNT;
}

// ------------------------------------------------------------------------------------------------
// Callback function to update the tiles shown on the viewPort.
gboolean on_menuBox_update(GtkWidget *widget, cairo_t *context, gpointer userData)
{
    // Get the GdkWindow from the widget
    GdkWindow *window = gtk_widget_get_window(widget);

    if (window)
    {
        // Create a Cairo context from the GdkWindow
        cairo_t *context = gdk_cairo_create(window);

        DrawMenuBoxBorders(context);

        DrawMovementArrows(context);

        // Draw the ZoomLevel tile.
        MenuBoxTile tileToDraw = TILE_UI_SWITCH_OFF;
        switch (GetViewPortZoomLevel())
        {
        case ZOOM_LEVEL_OFF:
            tileToDraw = TILE_UI_SWITCH_OFF;
            break;
        case ZOOM_LEVEL_MID:
            tileToDraw = TILE_UI_SWITCH_MID;
            break;
        case ZOOM_LEVEL_PEAK:
            tileToDraw = TILE_UI_SWITCH_ON;
            break;
        default:
            tileToDraw = TILE_UI_SWITCH_OFF;
        }
        gdk_cairo_set_source_pixbuf(context, menuBoxTiles[tileToDraw], ZOOM_ORIGIN_X, ZOOM_ORIGIN_Y);
        cairo_paint(context);

        // Draw the Exit tile.
        gdk_cairo_set_source_pixbuf(context, menuBoxTiles[TILE_SETTING_EXIT], EXIT_ORIGIN_X, EXIT_ORIGIN_Y);
        cairo_paint(context);


        // Clean up the Cairo context
        cairo_destroy(context);
    }
    return FALSE;
}

// ------------------------------------------------------------------------------------------------
// Callback function to track input on the viewPort.
gboolean on_menuBox_click(GtkWidget *widget, GdkEventButton *event, gpointer userData)
{
    Point clicked = {0};
    Point tileOrigin = {0};
    Actor *player = GetActor(0);

    // Get pixbuf tile that was clicked.
    clicked.x = (gint)(event->x);
    clicked.y = (gint)(event->y);

    // Move player actor and update viewPort if arrowIcon was clicked.
    Direction dirArrowClicked = WasMovementArrowClicked(&clicked);
    if (dirArrowClicked != DIR_NONE)
    {
        if (GetViewPortMode() == MODE_CHARACTER)
        {
            ActionWalk(player, dirArrowClicked);
            CenterViewPortOn(&player->position);
        }
        else
        {
            Point *newSelected = GetSelectedCell();

            newSelected->x += hMovement[dirArrowClicked];
            newSelected->y += vMovement[dirArrowClicked];

            SetSelectedCell(newSelected);
            CenterViewPortOn(GetSelectedCell());
        }
        gtk_widget_queue_draw(GTK_WIDGET(viewPort));
    }

    // Update viewPort's zoom if the zoomIcon was clicked.
    tileOrigin = {ZOOM_ORIGIN_X, ZOOM_ORIGIN_Y};
    if (IsWithinRectangle(&clicked, &tileOrigin, TILE_SIZE_MB, TILE_SIZE_MB))
    {
        guint zoomLevel = GetViewPortZoomLevel();

        zoomLevel = WrapValue(++zoomLevel, ZOOM_LEVEL_OFF, ZOOM_LEVEL_PEAK);
        SetViewPortZoomLevel((ZoomLevel)zoomLevel);
        ScaleTileForZoom();

        if (GetViewPortMode() == MODE_CHARACTER)
            CenterViewPortOn(&player->position);
        else
            CenterViewPortOn(GetSelectedCell());

        gtk_widget_queue_draw(GTK_WIDGET(viewPort));
        gtk_widget_queue_draw(GTK_WIDGET(menuBox));
    }

    // Exit the gtk main loop if the exit tile is clicked.
    tileOrigin = {EXIT_ORIGIN_X, EXIT_ORIGIN_Y};
    if (IsWithinRectangle(&clicked, &tileOrigin, TILE_SIZE_MB, TILE_SIZE_MB))
    {
        gtk_main_quit();
    }

    return TRUE;
}
