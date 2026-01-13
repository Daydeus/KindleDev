#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include "global.h"
#include "tile.h"
#include "actor.h"
#include "dungeonGeneration.h"
#include "menuBox.h"
#include "menuBoxLayout.h"

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

static void DrawMenuBoxBorders(cairo_t *context);
static void DoMenuStateClicked(Point* position);
static void DrawMenuStateSettings(cairo_t *context);
static void DoMenuStateSettingsInput(Point *inputPos);
static void DoMenuSettingsZoomClick(Point *inputPos);
static void DoMenuSettingsRefreshClick(Point *inputPos);

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
// Draws the contents of the menuBox when menuState is set to STATE_SETTINGS.
static void DrawMenuStateSettings(cairo_t *context)
{
    PangoLayout *layout = gtk_widget_create_pango_layout(GTK_WIDGET(menuBox), "");

    // Set layout's attributes.
    PangoAttrList *attr_list = pango_attr_list_new();
    PangoAttribute *color = pango_attr_foreground_new(65535, 65535, 65535); // WHITE
    pango_attr_list_insert(attr_list, color);
    pango_layout_set_attributes(layout, attr_list);

    // Loop through each piece of the layout and draw/print as necessary.
    for (guint i = 0; i < SETTINGS_COUNT; i++)
    {
        MenuLayout *settings = GetSettingsLayoutItem((SettingsUI)i);

        if (settings->isText)
        {
            pango_layout_set_text(layout, GetSettingsLayoutText((SettingsUI)i), -1);
            cairo_move_to(context, settings->layout.origin.x, settings->layout.origin.y);
            pango_cairo_show_layout(context, layout);
        }
        else
        {
            gdk_cairo_set_source_pixbuf(context, GetTileForMenuBoxSettings((SettingsUI)i),
                settings->layout.origin.x, settings->layout.origin.y);
            cairo_paint(context);
        }
    }

    g_object_unref(layout);
}

// ------------------------------------------------------------------------------------------------
// Callback function to update the tiles shown on the menuBox.
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
    // Loop through every item in the layout for menuState settings.
    for (guint i = 0; i < SETTINGS_COUNT; i++)
    {
        MenuLayout *settings = GetSettingsLayoutItem((SettingsUI)i);

        // If the inputPos is within the rectangle of the current item being checked.
        if (IsWithinRectangle(inputPos, &settings->layout.origin, settings->layout.width,
            settings->layout.height))
        {
            switch (i)
            {
            case SETTINGS_ZOOM_SWITCH:
                DoMenuSettingsZoomClick(inputPos);
                break;
            case SETTINGS_REFRESH_BUTTON:
                DoMenuSettingsRefreshClick(inputPos);
                break;
            case SETTINGS_EXIT_TEXT:
                gtk_main_quit();
                break;
            default:
                break;
            }
        }
    }
}

// ------------------------------------------------------------------------------------------------
// Update the viewPort zoom when the SETTINGS_ZOOM_SWITCH is clicked and queue viewPort and menuBox
// redraws.
static void DoMenuSettingsZoomClick(Point *inputPos)
{
    Actor *player = GetActor(0);
    gboolean zoomIsOn = GetViewPortZoom();

    zoomIsOn = !zoomIsOn;
    SetViewPortZoom(zoomIsOn);
    ScaleTileForZoom(zoomIsOn);

    CenterViewPortOn(&player->position);

    gtk_widget_queue_draw(GTK_WIDGET(viewPort));
    gtk_widget_queue_draw(GTK_WIDGET(menuBox));
}

// ------------------------------------------------------------------------------------------------
// Regenerate the dungeon and move all actors to random TERRAIN_FLOOR positions.
static void DoMenuSettingsRefreshClick(Point *inputPos)
{
    GenerateDungeon();
    PlaceAllActors();

    Actor *player = GetActor(0);
    CenterViewPortOn(&player->position);

    gtk_widget_queue_draw(GTK_WIDGET(viewPort));
}

// ------------------------------------------------------------------------------------------------
// Callback function to track input on the menuBox.
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
