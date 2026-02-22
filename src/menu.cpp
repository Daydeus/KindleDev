#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include "global.h"
#include "action.h"
#include "actor.h"
#include "dungeonCell.h"
#include "dungeonGeneration.h"
#include "dungeonMaster.h"
#include "fieldOfView.h"
#include "menu.h"
#include "menuLayout.h"
#include "tile.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------

#ifdef KINDLE_BUILD
    #define TAB_GAP 12
#else
    #define TAB_GAP 6
#endif

#define MENU_TAB_WIDTH          TILE_SIZE_MB + TAB_GAP * 2
#define MENU_TAB_HEIGHT         MENU_TABS_BOTTOM - (MENU_TABS_TOP) + TAB_GAP

// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

GtkDrawingArea *menu = NULL;
MenuState menuState = STATE_SETTINGS;

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

static void DrawMenuBorders(cairo_t *context);
static void DrawMenuTabs(cairo_t *context);
static void DrawPlayerHUD(cairo_t *context);
static void DrawMenuStateCharacter(cairo_t *context);
static void DrawMenuStateSettings(cairo_t *context);
static void DoMenuTabsClicked(Point* inputPos);
static void DoMenuStateCharacterInput(Point *inputPos);
static void DoMenuStateSettingsInput(Point *inputPos);
static void DoMenuSettingsZoomClick(Point *inputPos);
static void DoMenuSettingsSkipClick(Point *inputPos);
static void DoMenuSettingsFogOfWarClick(Point *inputPos);

// ------------------------------------------------------------------------------------------------
// Load GdkPixbuf tiles and initialize the menuBbox for the player.
void InitMenu(void)
{
    LoadMenuTiles();

    // Initialize menu.
    menu = GTK_DRAWING_AREA(gtk_drawing_area_new());
    gtk_widget_set_size_request(GTK_WIDGET(menu), MENU_WIDTH, MENU_HEIGHT);
    SetWidgetBgColor(GTK_WIDGET(menu), COLOR_BLACK);

    // Set up signals.
    g_signal_connect(menu, "expose_event", G_CALLBACK(on_menu_update), NULL);
    g_signal_connect(menu, "button_press_event", G_CALLBACK(on_menu_click), NULL);
    gtk_widget_set_events(GTK_WIDGET(menu), GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK);
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
// Draw the borders for the Menu.
static void DrawMenuBorders(cairo_t *context)
{
    // Menu edges for the North, East, South, and West directions.
    #define EDGE_N 0
    #define EDGE_E (MENU_WIDTH - TILE_SIZE_BORDER)
    #define EDGE_S (MENU_HEIGHT - TILE_SIZE_BORDER)
    #define EDGE_W 0

    guint tileVariant = 0;

    // Draw the borders across the top and bottom edges of the menu.
    for (guint i = TILE_SIZE_BORDER; i < MENU_WIDTH - TILE_SIZE_BORDER; i += TILE_SIZE_BORDER)
    {
        tileVariant = (i / TILE_SIZE_BORDER) % 3;
        gdk_cairo_set_source_pixbuf(context, borderTiles[TILE_BORDER_NORTH_1 + tileVariant], i, EDGE_N);
        cairo_paint(context);

        gdk_cairo_set_source_pixbuf(context, borderTiles[TILE_BORDER_SOUTH_1 + tileVariant], i, EDGE_S);
        cairo_paint(context);
    }

    // Draw the borders down the left and right edges of the menu.
    for (guint i = TILE_SIZE_BORDER; i < MENU_HEIGHT - TILE_SIZE_BORDER; i += TILE_SIZE_BORDER)
    {
        tileVariant = (i / TILE_SIZE_BORDER) % 3;
        gdk_cairo_set_source_pixbuf(context, borderTiles[TILE_BORDER_WEST_1 + tileVariant], EDGE_W, i);
        cairo_paint(context);

        gdk_cairo_set_source_pixbuf(context, borderTiles[TILE_BORDER_EAST_1 + tileVariant], EDGE_E, i);
        cairo_paint(context);
    }

    // Draw the corners of the menu.
    gdk_cairo_set_source_pixbuf(context, borderTiles[TILE_BORDER_CORNER_NORTH_EAST], EDGE_E, EDGE_N);
    cairo_paint(context);
    gdk_cairo_set_source_pixbuf(context, borderTiles[TILE_BORDER_CORNER_SOUTH_EAST], EDGE_E, EDGE_S);
    cairo_paint(context);
    gdk_cairo_set_source_pixbuf(context, borderTiles[TILE_BORDER_CORNER_SOUTH_WEST], EDGE_W, EDGE_S);
    cairo_paint(context);
    gdk_cairo_set_source_pixbuf(context, borderTiles[TILE_BORDER_CORNER_NORTH_WEST], EDGE_W, EDGE_N);
    cairo_paint(context);

    // Draw the top and bottom borders of the menuState tab bar.
    for (guint i = TILE_SIZE_BORDER; i < MENU_WIDTH - TILE_SIZE_BORDER; i += TILE_SIZE_BORDER)
    {
        guint tileVariant = (i / TILE_SIZE_BORDER) % 3;
        gdk_cairo_set_source_pixbuf(context, borderTiles[TILE_BORDER_MIDDLE_H_1 + tileVariant], i, MENU_TABS_TOP);
        cairo_paint(context);

        gdk_cairo_set_source_pixbuf(context, borderTiles[TILE_BORDER_MIDDLE_H_1 + tileVariant], i, MENU_TABS_BOTTOM);
        cairo_paint(context);
    }

    // Draw the T-post border pieces for menu state tab bar.
    gdk_cairo_set_source_pixbuf(context, borderTiles[TILE_BORDER_T_WEST], 0, MENU_TABS_TOP);
    cairo_paint(context);
    gdk_cairo_set_source_pixbuf(context, borderTiles[TILE_BORDER_T_EAST], MENU_WIDTH - TILE_SIZE_BORDER, MENU_TABS_TOP);
    cairo_paint(context);
    gdk_cairo_set_source_pixbuf(context, borderTiles[TILE_BORDER_T_WEST], 0, MENU_TABS_BOTTOM);
    cairo_paint(context);
    gdk_cairo_set_source_pixbuf(context, borderTiles[TILE_BORDER_T_EAST], MENU_WIDTH - TILE_SIZE_BORDER, MENU_TABS_BOTTOM);
    cairo_paint(context);

    #undef EDGE_N
    #undef EDGE_E
    #undef EDGE_S
    #undef EDGE_W
}

// ------------------------------------------------------------------------------------------------
// Draw the heads-up-display (HUD) for the player in the menu.
static void DrawPlayerHUD(cairo_t *context)
{
    PangoLayout *layout = gtk_widget_create_pango_layout(GTK_WIDGET(menu), "");
    Actor *player = GetActor(PLAYER_ACTOR_INDEX);

    // Set layout's attributes.
    PangoAttrList *attr_list = pango_attr_list_new();
    PangoAttribute *color = pango_attr_foreground_new(65535, 65535, 65535); // WHITE
    pango_attr_list_insert(attr_list, color);
    pango_layout_set_attributes(layout, attr_list);

    // Display the player's current health.
    char *text = g_strdup_printf("Player Health: %d", GetActorHealthCurrent(player));
    pango_layout_set_text(layout, text, -1);
    cairo_move_to(context, TILE_SIZE_BORDER, TILE_SIZE_BORDER / 2);
    pango_cairo_show_layout(context, layout);

    // Display the current floor number of the dungeon.
    text = g_strdup_printf("Floor: %d", GetDungeonFloor());
    pango_layout_set_text(layout, text, -1);
    cairo_move_to(context, TILE_SIZE_BORDER, TILE_SIZE_BORDER * 3/2);
    pango_cairo_show_layout(context, layout);

    g_object_unref(layout);
}

// ------------------------------------------------------------------------------------------------
// Draw the tabs for the menu's menuState.
static void DrawMenuTabs(cairo_t *context)
{
    Point cornerTop = {TILE_SIZE_MB + TAB_GAP, MENU_TABS_TOP + TAB_GAP};
    Point cornerBottom = {TILE_SIZE_MB + TAB_GAP, MENU_TABS_BOTTOM - TAB_GAP};
    Point stateIcon = {TILE_SIZE_MB / 4, MENU_TABS_TOP + TILE_SIZE_MB / 2};

    // Draw the tab frame for each of the menu states.
    for (gint state = 0; state < STATE_COUNT; state++)
    {
        gdk_cairo_set_source_pixbuf(context, GetTileForMenuState((MenuState)state), stateIcon.x, stateIcon.y);
        cairo_paint(context);

        gdk_cairo_set_source_pixbuf(context, borderTiles[TILE_BORDER_MIDDLE_V_1], cornerTop.x, cornerTop.y + TILE_SIZE_BORDER);
        cairo_paint(context);
        gdk_cairo_set_source_pixbuf(context, borderTiles[TILE_BORDER_MIDDLE_V_1], cornerBottom.x, cornerBottom.y - TILE_SIZE_BORDER);
        cairo_paint(context);
        gdk_cairo_set_source_pixbuf(context, borderTiles[TILE_BORDER_T_NORTH], cornerTop.x, cornerTop.y);
        cairo_paint(context);
        gdk_cairo_set_source_pixbuf(context, borderTiles[TILE_BORDER_T_SOUTH], cornerBottom.x, cornerBottom.y);
        cairo_paint(context);

        // Cover the bottom edge of the tab bar to help indicate which tab is selected.
        if (state == GetMenuState())
        {
            cairo_set_source_rgb(context, 0.0, 0.0, 0.0); // COLOR_BLACK
            cairo_rectangle(context, cornerBottom.x - TILE_SIZE_MB - 1, cornerBottom.y + TAB_GAP, TILE_SIZE_MB + TAB_GAP, TILE_SIZE_BORDER);
            cairo_fill(context);
        }

        stateIcon.x += MENU_TAB_WIDTH;
        cornerTop.x += MENU_TAB_WIDTH;
        cornerBottom.x += MENU_TAB_WIDTH;
    }
}

// ------------------------------------------------------------------------------------------------
// Draws the contents of the menu when menuState is set to STATE_CHARACTER.
static void DrawMenuStateCharacter(cairo_t *context)
{
    PangoLayout *layout = gtk_widget_create_pango_layout(GTK_WIDGET(menu), "");

    // Set layout's attributes.
    PangoAttrList *attr_list = pango_attr_list_new();
    PangoAttribute *color = pango_attr_foreground_new(65535, 65535, 65535); // WHITE
    pango_attr_list_insert(attr_list, color);
    pango_layout_set_attributes(layout, attr_list);

    // Loop through each piece of the layout and draw/print as necessary.
    for (guint i = 0; i < MB_CHARACTER_COUNT; i++)
    {
        MenuLayout *menuItem = GetCharacterLayoutItem((CharacterUI)i);

        if (menuItem->isText)
        {
            pango_layout_set_text(layout, GetCharacterLayoutText((CharacterUI)i), -1);
            cairo_move_to(context, menuItem->layout.origin.x, menuItem->layout.origin.y);
            pango_cairo_show_layout(context, layout);
        }
        else
        {
            gdk_cairo_set_source_pixbuf(context, GetTileForMenuCharacter((CharacterUI)i),
                menuItem->layout.origin.x, menuItem->layout.origin.y);
            cairo_paint(context);
        }
    }

    g_object_unref(layout);
}

// ------------------------------------------------------------------------------------------------
// Draws the contents of the menu when menuState is set to STATE_SETTINGS.
static void DrawMenuStateSettings(cairo_t *context)
{
    PangoLayout *layout = gtk_widget_create_pango_layout(GTK_WIDGET(menu), "");

    // Set layout's attributes.
    PangoAttrList *attr_list = pango_attr_list_new();
    PangoAttribute *color = pango_attr_foreground_new(65535, 65535, 65535); // WHITE
    PangoAttribute *size = pango_attr_size_new(12 * PANGO_SCALE); // 12-point font
    pango_attr_list_insert(attr_list, color);
    pango_attr_list_insert(attr_list, size);
    pango_layout_set_attributes(layout, attr_list);

    // Loop through each piece of the layout and draw/print as necessary.
    for (guint i = 0; i < MB_SETTINGS_COUNT; i++)
    {
        MenuLayout *menuItem = GetSettingsLayoutItem((SettingsUI)i);

        if (menuItem->isText)
        {
            pango_layout_set_text(layout, GetSettingsLayoutText((SettingsUI)i), -1);
            cairo_move_to(context, menuItem->layout.origin.x, menuItem->layout.origin.y);
            pango_cairo_show_layout(context, layout);
        }
        else
        {
            gdk_cairo_set_source_pixbuf(context, GetTileForMenuSettings((SettingsUI)i),
                menuItem->layout.origin.x, menuItem->layout.origin.y);
            cairo_paint(context);
        }
    }

    g_object_unref(layout);
}

// ------------------------------------------------------------------------------------------------
// Callback function to update the tiles shown on the menu.
gboolean on_menu_update(GtkWidget *widget, cairo_t *context, gpointer userData)
{
    // Get the GdkWindow from the widget
    GdkWindow *window = gtk_widget_get_window(widget);

    if (window)
    {
        // Create a Cairo context from the GdkWindow
        cairo_t *context = gdk_cairo_create(window);

        DrawMenuBorders(context);
        DrawMenuTabs(context);
        DrawPlayerHUD(context);

        switch (GetMenuState())
        {
        case STATE_INSPECT:
            break;
        case STATE_CHARACTER:
            DrawMenuStateCharacter(context);
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
// Process input for when the tabs of different menu states are clicked.
static void DoMenuTabsClicked(Point* inputPos)
{
    Point currentTab = {0, MENU_TABS_TOP + TAB_GAP};

    // Loop through each of the tabs and check if they were clicked.
    for (gint i = 0; i < STATE_COUNT; i++)
    {
        if (IsWithinRectangle(inputPos, &currentTab, MENU_TAB_WIDTH, MENU_TAB_HEIGHT))
            SetMenuState((MenuState)i);

        currentTab.x += MENU_TAB_WIDTH;
    }

    gtk_widget_queue_draw(GTK_WIDGET(menu));
}

// ------------------------------------------------------------------------------------------------
// Process input for the menu when in menuState STATE_CHARACTER.
static void DoMenuStateCharacterInput(Point *inputPos)
{
    // Loop through every item in the layout for menuState character.
    for (guint i = 0; i < MB_CHARACTER_COUNT; i++)
    {
        MenuLayout *menuItem = GetCharacterLayoutItem((CharacterUI)i);

        // If the inputPos is within the rectangle of the current item being checked.
        if (IsWithinRectangle(inputPos, &menuItem->layout.origin, menuItem->layout.width,
            menuItem->layout.height))
        {
            switch (i)
            {
            case MB_CHARACTER_TERRAIN_FLIP_BTTN:
                if (GetSelectedCellStatus() == STATUS_UNLOCKED)
                {
                    SetActionForPlayer(ACTION_TERRAIN_FLIP);
                    ProcessTurn(NULL);
                }
                else if (GetSelectedCellStatus() == STATUS_LOCKED)
                {
                    SetSelectedCellStatus(STATUS_UNLOCKED);
                    gtk_widget_queue_draw(GTK_WIDGET(viewPort));
                    SetActionForPlayer(ACTION_NONE);
                }

                break;
            default:
                break;
            }
        }
    }
}

// ------------------------------------------------------------------------------------------------
// Process input for the menu when in menuState STATE_SETTINGS.
static void DoMenuStateSettingsInput(Point *inputPos)
{
    // Loop through every item in the layout for menuState settings.
    for (guint i = 0; i < MB_SETTINGS_COUNT; i++)
    {
        MenuLayout *settings = GetSettingsLayoutItem((SettingsUI)i);

        // If the inputPos is within the rectangle of the current item being checked.
        if (IsWithinRectangle(inputPos, &settings->layout.origin, settings->layout.width,
            settings->layout.height))
        {
            switch (i)
            {
            case MB_SETTINGS_ZOOM_SWITCH:
                DoMenuSettingsZoomClick(inputPos);
                break;
            case MB_SETTINGS_SKIP_BUTTON:
                DoMenuSettingsSkipClick(inputPos);
                break;
            case MB_SETTINGS_FOGOFWAR_BUTTON:
                DoMenuSettingsFogOfWarClick(inputPos);
                break;
            case MB_SETTINGS_EXIT_BUTTON:
                gtk_main_quit();
                break;
            default:
                break;
            }
        }
    }
}

// ------------------------------------------------------------------------------------------------
// Update the viewPort zoom when the SETTINGS_ZOOM_SWITCH is clicked and queue viewPort and menu
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
    gtk_widget_queue_draw(GTK_WIDGET(menu));
}

// ------------------------------------------------------------------------------------------------
// Regenerate the dungeon and move all actors to random TERRAIN_FLOOR positions.
static void DoMenuSettingsSkipClick(Point *inputPos)
{
    SetActionForPlayer(ACTION_ADVANCE_FLOOR);
    ProcessTurn(NULL);
}

// ------------------------------------------------------------------------------------------------
// Toggles whether the Fog of War is on or off.
static void DoMenuSettingsFogOfWarClick(Point *inputPos)
{
    SetFogOfWarStatus(!GetFogOfWarStatus());

    gtk_widget_queue_draw(GTK_WIDGET(viewPort));
    gtk_widget_queue_draw(GTK_WIDGET(menu));
}

// ------------------------------------------------------------------------------------------------
// Callback function to track input on the menu.
gboolean on_menu_click(GtkWidget *widget, GdkEventButton *event, gpointer userData)
{
    // Get pixbuf tile that was clicked.
    Point clicked = {(gint)(event->x), (gint)(event->y)};
    Point tabsOrigin = {0, MENU_TABS_TOP + TAB_GAP};

    // Check if a menu state icon was clicked.
    if (IsWithinRectangle(&clicked, &tabsOrigin, MENU_WIDTH, MENU_TAB_HEIGHT))
    {
        DoMenuTabsClicked(&clicked);
    }
    else
    {
        switch (GetMenuState())
        {
        case STATE_INSPECT:
            break;
        case STATE_CHARACTER:
            DoMenuStateCharacterInput(&clicked);
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
