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
MenuState menuState = STATE_SETTINGS;

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

static void DrawMovementArrows(cairo_t *context);
static void DrawMenuBoxBorders(cairo_t *context);
static void DoMenuStateClicked(Point* position);
static Direction WasMovementArrowClicked(Point *position);
static void DrawMenuStateSettings(cairo_t *context);
static void DoMenuStateSettingsInput(Point *inputPos);

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
// Gets the current menuState.
MenuState GetMenuState(void)
{
    return menuState;
}

// ------------------------------------------------------------------------------------------------
// Sets the current menuState.
void SetMenuState(MenuState state)
{
    menuState = state;
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

    // Draw the borders across the top and bottom edges of the menuBox.
    for (guint i = TILE_SIZE_MB; i < MENU_BOX_WIDTH - TILE_SIZE_MB * 2; i += TILE_SIZE_MB)
    {
        tileVariant = (i / TILE_SIZE_MB) % 3;
        gdk_cairo_set_source_pixbuf(context, borderTiles[TILE_BORDER_NORTH_1 + tileVariant], i, EDGE_N);
        cairo_paint(context);

        gdk_cairo_set_source_pixbuf(context, borderTiles[TILE_BORDER_SOUTH_1 + tileVariant], i, EDGE_S);
        cairo_paint(context);
    }

    // Draw the borders down the left edge of the menuBox.
    for (guint i = TILE_SIZE_MB; i < MENU_BOX_HEIGHT - TILE_SIZE_MB; i += TILE_SIZE_MB)
    {
        tileVariant = (i / TILE_SIZE_MB) % 3;
        gdk_cairo_set_source_pixbuf(context, borderTiles[TILE_BORDER_WEST_1 + tileVariant], EDGE_W, i);
        cairo_paint(context);
    }

    // Draw the menuState icon borders on the right edge of the menuBox.
    for (guint i = 0; i < MENU_BOX_HEIGHT; i += TILE_SIZE_MB)
    {
        BorderTile tileLeft = IsValueEven(i / TILE_SIZE_MB) ? TILE_BORDER_CORNER_NORTH_WEST : TILE_BORDER_CORNER_SOUTH_WEST;
        BorderTile tileRight = IsValueEven(i / TILE_SIZE_MB) ? TILE_BORDER_CORNER_NORTH_EAST : TILE_BORDER_CORNER_SOUTH_EAST;

        gdk_cairo_set_source_pixbuf(context, borderTiles[tileLeft], EDGE_E - TILE_SIZE_MB, i);
        cairo_paint(context);
        gdk_cairo_set_source_pixbuf(context, borderTiles[tileRight], EDGE_E, i);
        cairo_paint(context);
    }

    // Draw the remaining corners.
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
// Draw the icons for the menuBox's menuState.
static void DrawMenuStateIcons(cairo_t *context)
{
    // Draw the menuState icons centered in the border boxes drawn by DrawMenuBoxBorders().
    Point pixel = {MENU_BOX_WIDTH - TILE_SIZE_MB * 3 / 2, TILE_SIZE_MB / 2};

    for (guint i = 0; i < STATE_COUNT; i++)
    {
        guint tileToDraw = i;

        // Icons are ordered in tileset for looping; they just need the appropriate offset applied.
        if (i == GetMenuState())
            tileToDraw += TILE_BORDER_ICON_ON_OFFSET;
        else
            tileToDraw += TILE_BORDER_ICON_OFF_OFFSET;

        gdk_cairo_set_source_pixbuf(context, borderTiles[tileToDraw], pixel.x, pixel.y);
        cairo_paint(context);

        pixel.y += TILE_SIZE_MB * 2;
    }
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
// Uses the given position to determine which menuBox state icon was selected. If the menuBox state
// was differenct thatn the current state, changes the state and queues a redraw for the menuBox.
static void DoMenuStateClicked(Point* position)
{
    guint stateIndex = 0;

    // The menuState indicators are two tiles wide and span the whole vertical length of the menuBox
    // on the right side.
    for (gint i = 0; i < MENU_BOX_HEIGHT; i += TILE_SIZE_MB * 2)
    {
        Point menuStateIcon = {MENU_BOX_WIDTH - TILE_SIZE_MB * 2, i};

        if (IsWithinRectangle(position, &menuStateIcon, TILE_SIZE_MB * 2, TILE_SIZE_MB * 2))
        {
            if (stateIndex != GetMenuState())
            {
                SetMenuState(MenuState(stateIndex));
                gtk_widget_queue_draw(GTK_WIDGET(menuBox));
            }
        }
        stateIndex++;
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
// Draws the contents of the menuBox when menuState is set to STATE_SETTINGS.
static void DrawMenuStateSettings(cairo_t *context)
{
    DrawMovementArrows(context);

    // Draw the UI switch tile indicating that viewPort zoom is active.
    if (GetViewPortZoom())
        gdk_cairo_set_source_pixbuf(context, menuBoxTiles[TILE_UI_SWITCH_ON], ZOOM_ORIGIN_X, ZOOM_ORIGIN_Y);
    else
        gdk_cairo_set_source_pixbuf(context, menuBoxTiles[TILE_UI_SWITCH_OFF], ZOOM_ORIGIN_X, ZOOM_ORIGIN_Y);
    cairo_paint(context);

    // Draw the Exit tile.
    gdk_cairo_set_source_pixbuf(context, menuBoxTiles[TILE_SETTING_EXIT], EXIT_ORIGIN_X, EXIT_ORIGIN_Y);
    cairo_paint(context);
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
        DrawMenuStateIcons(context);

        switch (GetMenuState())
        {
        case STATE_INSPECT:
            break;
        case STATE_CHARACTER:
            break;
        case STATE_INVENTORY:
            break;
        case STATE_LOGBOOK:
            break;
        case STATE_SETTINGS:
            DrawMenuStateSettings(context);
            break;
        default:
            break;
        }

        // Clean up the Cairo context
        cairo_destroy(context);
    }
    return FALSE;
}

// ------------------------------------------------------------------------------------------------
// Process input for the menuBox when in menuState STATE_SETTINGS.
static void DoMenuStateSettingsInput(Point *inputPos)
{
    Point tileOrigin = {0};
    Actor *player = GetActor(0);

    // Move player actor and update viewPort if arrowIcon was clicked.
    Direction dirArrowClicked = WasMovementArrowClicked(inputPos);
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
    if (IsWithinRectangle(inputPos, &tileOrigin, TILE_SIZE_MB, TILE_SIZE_MB))
    {
        gboolean zoomIsOn = GetViewPortZoom();

        zoomIsOn = !zoomIsOn;
        SetViewPortZoom(zoomIsOn);
        ScaleTileForZoom(zoomIsOn);

        if (GetViewPortMode() == MODE_CHARACTER)
            CenterViewPortOn(&player->position);
        else
            CenterViewPortOn(GetSelectedCell());

        gtk_widget_queue_draw(GTK_WIDGET(viewPort));
        gtk_widget_queue_draw(GTK_WIDGET(menuBox));
    }

    // Exit the gtk main loop if the exit tile is clicked.
    tileOrigin = {EXIT_ORIGIN_X, EXIT_ORIGIN_Y};
    if (IsWithinRectangle(inputPos, &tileOrigin, TILE_SIZE_MB, TILE_SIZE_MB))
    {
        gtk_main_quit();
    }

}

// ------------------------------------------------------------------------------------------------
// Callback function to track input on the viewPort.
gboolean on_menuBox_click(GtkWidget *widget, GdkEventButton *event, gpointer userData)
{
    // Get pixbuf tile that was clicked.
    Point clicked = {(gint)(event->x), (gint)(event->y)};
    Point tileOrigin = {MENU_BOX_WIDTH - TILE_SIZE_MB * 2, 0};

    // Check if a menuBox state icon was clicked.
    if (IsWithinRectangle(&clicked, &tileOrigin, TILE_SIZE_MB * 2, MENU_BOX_HEIGHT))
    {
        DoMenuStateClicked(&clicked);
    }
    else
    {
        switch (GetMenuState())
        {
        case STATE_INSPECT:
            break;
        case STATE_CHARACTER:
            break;
        case STATE_INVENTORY:
            break;
        case STATE_LOGBOOK:
            break;
        case STATE_SETTINGS:
            DoMenuStateSettingsInput(&clicked);
            break;
        default:
            break;
        }
    }

    return TRUE;
}
