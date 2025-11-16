#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include "global.h"
#include "actor.h"
#include "playerController.h"
#include "viewPort.h"
#include "data/icons/icon_arrow_north.h"
#include "data/icons/icon_arrow_northEast.h"
#include "data/icons/icon_arrow_east.h"
#include "data/icons/icon_arrow_southEast.h"
#include "data/icons/icon_arrow_south.h"
#include "data/icons/icon_arrow_southWest.h"
#include "data/icons/icon_arrow_west.h"
#include "data/icons/icon_arrow_northWest.h"
#include "data/icons/icon_setting_off.h"
#include "data/icons/icon_setting_mid.h"
#include "data/icons/icon_setting_on.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

GdkPixbuf *icons[ICON_COUNT] = {NULL};
GtkDrawingArea *playerControlsBox = NULL;

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

static Direction WasMovementArrowClicked(Point *position);
static const guint8* GetIconImageData(Icon icon);

// ------------------------------------------------------------------------------------------------
// Load GdkPixbuf icons and initialize the controls box for the player.
void InitControlsBox(void)
{
    LoadIcons();

    // Initialize playerControlsBox.
    playerControlsBox = GTK_DRAWING_AREA(gtk_drawing_area_new());
    gtk_widget_set_size_request(GTK_WIDGET(playerControlsBox), CONTROLS_BOX_WIDTH, CONTROLS_BOX_HEIGHT);
    SetWidgetBgColor(GTK_WIDGET(playerControlsBox), COLOR_WHITE);

    // Set up signals.
    g_signal_connect(playerControlsBox, "expose_event", G_CALLBACK(on_playerControlsBox_update), NULL);
    g_signal_connect(playerControlsBox, "button_press_event", G_CALLBACK(on_playerControlsBox_click), NULL);
    gtk_widget_set_events(GTK_WIDGET(playerControlsBox), GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK);
}

// ------------------------------------------------------------------------------------------------
// Returns the direction if an arrow icon was clicked; otherwise it returns DIR_COUNT;
static Direction WasMovementArrowClicked(Point *position)
{

    for (guint i = 0; i < DIR_COUNT; i++)
    {
        Point iconOrigin = {ARROWS_START_X + ICON_SIZE, ARROWS_START_Y + ICON_SIZE};

        iconOrigin.x += (hMovement[i] * ICON_SIZE);
        iconOrigin.y += (vMovement[i] * ICON_SIZE);

        if (IsWithinRectangle(position, &iconOrigin, ICON_SIZE, ICON_SIZE) == TRUE)
            return (Direction)i;
    }

    return DIR_COUNT;
}

// ------------------------------------------------------------------------------------------------
// Returns the array of image data required for gdk_pixbuf_new_from_inline.
static const guint8* GetIconImageData(Icon icon)
{
    switch (icon)
    {
    case ICON_ARROW_NORTH:
        return icon_arrow_north;
    case ICON_ARROW_NORTH_EAST:
        return icon_arrow_northEast;
    case ICON_ARROW_EAST:
        return icon_arrow_east;
    case ICON_ARROW_SOUTH_EAST:
        return icon_arrow_southEast;
    case ICON_ARROW_SOUTH:
        return icon_arrow_south;
    case ICON_ARROW_SOUTH_WEST:
        return icon_arrow_southWest;
    case ICON_ARROW_WEST:
        return icon_arrow_west;
    case ICON_ARROW_NORTH_WEST:
        return icon_arrow_northWest;
    case ICON_SETTING_OFF:
        return icon_setting_off;
    case ICON_SETTING_MID:
        return icon_setting_mid;
    case ICON_SETTING_ON:
        return icon_setting_on;
    default:
        return NULL;
    }
}

// ------------------------------------------------------------------------------------------------
// Read image data into the GdkPixbufs icons array.
void LoadIcons(void)
{
    GError * error = NULL;
    for (guint i = 0; i < ICON_COUNT; i++)
    {
        icons[i] = gdk_pixbuf_new_from_inline(-1, GetIconImageData((Icon)i), FALSE, &error);
        icons[i] = gdk_pixbuf_scale_simple(icons[i], ICON_SIZE, ICON_SIZE, GDK_INTERP_NEAREST);
    }
}

// ------------------------------------------------------------------------------------------------
// Free the GdkPixbufs for the icons array.
void FreeIcons(void)
{
    // Free memory used by GdkPixbufs.
    for (guint i = 0; i < ICON_COUNT; i++)
    {
        g_object_unref(icons[i]);
    }
}

// ------------------------------------------------------------------------------------------------
// Callback function to update the tiles shown on the viewPort.
gboolean on_playerControlsBox_update(GtkWidget *widget, cairo_t *context, gpointer userData)
{
    // Get the GdkWindow from the widget
    GdkWindow *window = gtk_widget_get_window(widget);

    if (window)
    {
        // Create a Cairo context from the GdkWindow
        cairo_t *context = gdk_cairo_create(window);

        Icon iconToDraw = ICON_SETTING_OFF;

        // Draw the arrow movement icons.
        for (gint i = 0; i < DIR_COUNT; i++)
        {
            Point position = {ARROWS_START_X + ICON_SIZE, ARROWS_START_Y + ICON_SIZE};

            position.x += (hMovement[i] * ICON_SIZE);
            position.y += (vMovement[i] * ICON_SIZE);

            gdk_cairo_set_source_pixbuf(context, icons[i], position.x, position.y);
            cairo_paint(context);
        }

        // Draw the ZoomLevel icon.
        switch (GetViewPortZoomLevel())
        {
        case ZOOM_LEVEL_OFF:
            iconToDraw = ICON_SETTING_OFF;
            break;
        case ZOOM_LEVEL_MID:
            iconToDraw = ICON_SETTING_MID;
            break;
        case ZOOM_LEVEL_PEAK:
            iconToDraw = ICON_SETTING_ON;
            break;
        default:
            iconToDraw = ICON_SETTING_OFF;
        }
        gdk_cairo_set_source_pixbuf(context, icons[iconToDraw], ZOOM_START_X, ZOOM_START_Y);
        cairo_paint(context);

        // Clean up the Cairo context
        cairo_destroy(context);
    }
    return FALSE;
}

// ------------------------------------------------------------------------------------------------
// Callback function to track input on the viewPort.
gboolean on_playerControlsBox_click(GtkWidget *widget, GdkEventButton *event, gpointer userData)
{
    Point clicked = {0};
    Actor *player = GetActor(0);

    // Get pixbuf tile that was clicked.
    clicked.x = (gint)(event->x);
    clicked.y = (gint)(event->y);

    // Move player actor and update viewPort if arrowIcon was clicked.
    Direction dirArrowClicked = WasMovementArrowClicked(&clicked);
    if (dirArrowClicked != DIR_NONE)
    {
        ActionWalk(player, dirArrowClicked);
        CenterViewPortOn(player->position.x, player->position.y);
        gtk_widget_queue_draw(GTK_WIDGET(viewPort));
    }

    // Update viewPort's zoom if the zoomIcon was clicked.
    if (clicked.x > ZOOM_START_X && clicked.x < ZOOM_START_X + ICON_SIZE
        && clicked.y > ZOOM_START_Y && clicked.y < ZOOM_START_Y + ICON_SIZE)
    {
        guint zoomLevel = GetViewPortZoomLevel();

        zoomLevel = (zoomLevel == ZOOM_LEVEL_PEAK) ? ZOOM_LEVEL_OFF : zoomLevel + 1;

        SetViewPortZoomLevel((ZoomLevel)zoomLevel);
        ScaleTileForZoom();
        CenterViewPortOn(player->position.x, player->position.y);
        gtk_widget_queue_draw(GTK_WIDGET(viewPort));
        gtk_widget_queue_draw(GTK_WIDGET(playerControlsBox));
    }

    return TRUE;
}
