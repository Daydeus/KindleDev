#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include "global.h"
#include "actor.h"
#include "menuBox.h"
#include "viewPort.h"
#include "data/tilesetMenuBoxSettings.h"

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
GtkDrawingArea *menuBox = NULL;

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

static Direction WasMovementArrowClicked(Point *position);

// ------------------------------------------------------------------------------------------------
// Load GdkPixbuf icons and initialize the controls box for the player.
void InitControlsBox(void)
{
    LoadIcons();

    // Initialize menuBox.
    menuBox = GTK_DRAWING_AREA(gtk_drawing_area_new());
    gtk_widget_set_size_request(GTK_WIDGET(menuBox), MENU_BOX_WIDTH, MENU_BOX_HEIGHT);
    SetWidgetBgColor(GTK_WIDGET(menuBox), COLOR_GREY_LIGHT);

    // Set up signals.
    g_signal_connect(menuBox, "expose_event", G_CALLBACK(on_menuBox_update), NULL);
    g_signal_connect(menuBox, "button_press_event", G_CALLBACK(on_menuBox_click), NULL);
    gtk_widget_set_events(GTK_WIDGET(menuBox), GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK);
}

// ------------------------------------------------------------------------------------------------
// Returns the direction if an arrow icon was clicked; otherwise it returns DIR_COUNT;
static Direction WasMovementArrowClicked(Point *position)
{

    for (guint i = 0; i < DIR_COUNT; i++)
    {
        Point iconOrigin = {ARROWS_ORIGIN_X + ICON_SIZE, ARROWS_ORIGIN_Y + ICON_SIZE};

        iconOrigin.x += (hMovement[i] * ICON_SIZE);
        iconOrigin.y += (vMovement[i] * ICON_SIZE);

        if (IsWithinRectangle(position, &iconOrigin, ICON_SIZE, ICON_SIZE) == TRUE)
            return (Direction)i;
    }

    return DIR_COUNT;
}

// ------------------------------------------------------------------------------------------------
// Read image data into the GdkPixbufs icons array.
void LoadIcons(void)
{
    GdkPixbuf *source = NULL;
    GError * error = NULL;

    source = gdk_pixbuf_new_from_inline(-1, tilesetMenuBoxSettings, FALSE, &error);

    for (guint i = 0; i < ICON_COUNT; i++)
    {
        guint pixelX = (i % TILESET_WIDTH) * TILE_SIZE_16;
        guint pixelY = (i / TILESET_WIDTH) * TILE_SIZE_16;

        icons[i] = gdk_pixbuf_new_subpixbuf(source, pixelX, pixelY, TILE_SIZE_16, TILE_SIZE_16);
        icons[i] = gdk_pixbuf_scale_simple(icons[i], ICON_SIZE, ICON_SIZE, GDK_INTERP_NEAREST);
    }

    g_object_unref(source);
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
gboolean on_menuBox_update(GtkWidget *widget, cairo_t *context, gpointer userData)
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
            Point position = {ARROWS_ORIGIN_X + ICON_SIZE, ARROWS_ORIGIN_Y + ICON_SIZE};

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
        gdk_cairo_set_source_pixbuf(context, icons[iconToDraw], ZOOM_ORIGIN_X, ZOOM_ORIGIN_Y);
        cairo_paint(context);

        // Draw the Exit icon.
        gdk_cairo_set_source_pixbuf(context, icons[ICON_SETTING_EXIT], EXIT_ORIGIN_X, EXIT_ORIGIN_Y);
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
    Point iconOrigin = {0};
    Actor *player = GetActor(0);

    // Get pixbuf tile that was clicked.
    clicked.x = (gint)(event->x);
    clicked.y = (gint)(event->y);

    // Move player actor and update viewPort if arrowIcon was clicked.
    Direction dirArrowClicked = WasMovementArrowClicked(&clicked);
    if (dirArrowClicked != DIR_NONE)
    {
        ActionWalk(player, dirArrowClicked);
        CenterViewPortOn(&player->position);
        gtk_widget_queue_draw(GTK_WIDGET(viewPort));
    }

    // Update viewPort's zoom if the zoomIcon was clicked.
    iconOrigin = {ZOOM_ORIGIN_X, ZOOM_ORIGIN_Y};
    if (IsWithinRectangle(&clicked, &iconOrigin, ICON_SIZE, ICON_SIZE))
    {
        guint zoomLevel = GetViewPortZoomLevel();

        zoomLevel = WrapValue(++zoomLevel, ZOOM_LEVEL_OFF, ZOOM_LEVEL_PEAK);
        SetViewPortZoomLevel((ZoomLevel)zoomLevel);
        ScaleTileForZoom();
        CenterViewPortOn(&player->position);

        gtk_widget_queue_draw(GTK_WIDGET(viewPort));
        gtk_widget_queue_draw(GTK_WIDGET(menuBox));
    }

    // Exit the gtk main loop if the exit icon is clicked.
    iconOrigin = {EXIT_ORIGIN_X, EXIT_ORIGIN_Y};
    if (IsWithinRectangle(&clicked, &iconOrigin, ICON_SIZE, ICON_SIZE))
    {
        gtk_main_quit();
    }

    return TRUE;
}
