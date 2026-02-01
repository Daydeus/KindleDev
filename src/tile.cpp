#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include "actor.h"
#include "dungeonCell.h"
#include "fieldOfView.h"
#include "global.h"
#include "menuBoxLayout.h"
#include "tile.h"
#include "viewPort.h"
#include "data/tilesetActor.h"
#include "data/tilesetBorder.h"
#include "data/tilesetColorFill.h"
#include "data/tilesetDungeonCave.h"
#include "data/tilesetDungeonCaveDark.h"
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

GdkPixbuf *actorTiles[TILE_COUNT_ACTOR] = {NULL};
GdkPixbuf *borderTiles[TILE_COUNT_BORDER] = {NULL};
GdkPixbuf *colorFillTiles[COLOR_COUNT_ALL] = {NULL};
GdkPixbuf *dungeonLightTiles[TILE_COUNT_VP] = {NULL};
GdkPixbuf *dungeonDarkTiles[TILE_COUNT_VP] = {NULL};
GdkPixbuf *menuBoxTiles[TILE_COUNT_MB] = {NULL};

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

static GdkPixbuf* GetDungeonLightTile(DungeonTile tile);
static GdkPixbuf* GetDungeonDarkTile(DungeonTile tile);
static DungeonTile GetWallTile(Point *position);

// ------------------------------------------------------------------------------------------------
// Read image data into the GdkPixbufs actorTiles array.
void LoadActorTiles(void)
{
    GdkPixbuf *source = NULL;
    GError * error = NULL;

    source = gdk_pixbuf_new_from_inline(-1, tilesetActor, FALSE, &error);

    for (guint i = 0; i < TILE_COUNT_ACTOR; i++)
    {
        guint pixelX = (i % TILESET_WIDTH) * TILE_SIZE_16;
        guint pixelY = (i / TILESET_WIDTH) * TILE_SIZE_16;

        actorTiles[i] = gdk_pixbuf_new_subpixbuf(source, pixelX, pixelY, TILE_SIZE_16, TILE_SIZE_16);
        actorTiles[i] = gdk_pixbuf_scale_simple(actorTiles[i], TILE_SIZE_BORDER, TILE_SIZE_BORDER,
            GDK_INTERP_NEAREST);
    }

    g_object_unref(source);
}

// ------------------------------------------------------------------------------------------------
// Free the GdkPixbufs for the actorTiles array.
void FreeActorTiles(void)
{
    // Free memory used by GdkPixbufs.
    for (guint i = 0; i < TILE_COUNT_ACTOR; i++)
    {
        g_object_unref(actorTiles[i]);
    }
}

// ------------------------------------------------------------------------------------------------
// Read image data into the GdkPixbufs borderTiles array.
void LoadBorderTiles(void)
{
    GdkPixbuf *source = NULL;
    GError * error = NULL;

    source = gdk_pixbuf_new_from_inline(-1, tilesetBorder, FALSE, &error);

    for (guint i = 0; i < TILE_COUNT_BORDER; i++)
    {
        guint pixelX = (i % TILESET_WIDTH) * TILE_SIZE_16;
        guint pixelY = (i / TILESET_WIDTH) * TILE_SIZE_16;

        borderTiles[i] = gdk_pixbuf_new_subpixbuf(source, pixelX, pixelY, TILE_SIZE_16, TILE_SIZE_16);
        borderTiles[i] = gdk_pixbuf_scale_simple(borderTiles[i], TILE_SIZE_BORDER, TILE_SIZE_BORDER,
            GDK_INTERP_NEAREST);
    }

    g_object_unref(source);
}

// ------------------------------------------------------------------------------------------------
// Free the GdkPixbufs for the borderTiles array.
void FreeBorderTiles(void)
{
    // Free memory used by GdkPixbufs.
    for (guint i = 0; i < TILE_COUNT_BORDER; i++)
    {
        g_object_unref(borderTiles[i]);
    }
}

// ------------------------------------------------------------------------------------------------
// Read image data into the GdkPixbufs colorFillTiles array.
void LoadColorFillTiles(void)
{
    GdkPixbuf *source = NULL;
    GError * error = NULL;

    source = gdk_pixbuf_new_from_inline(-1, tilesetColorFill, FALSE, &error);

    for (guint i = 0; i < COLOR_COUNT_ALL; i++)
    {
        guint pixelX = (i % TILESET_WIDTH) * TILE_SIZE_16;
        guint pixelY = (i / TILESET_WIDTH) * TILE_SIZE_16;

        colorFillTiles[i] = gdk_pixbuf_new_subpixbuf(source, pixelX, pixelY, TILE_SIZE_16, TILE_SIZE_16);
    }

    g_object_unref(source);
}

// ------------------------------------------------------------------------------------------------
// Free the GdkPixbufs for the colorFillTiles array.
void FreeColorFillTiles(void)
{
    // Free memory used by GdkPixbufs.
    for (guint i = 0; i < COLOR_COUNT_ALL; i++)
    {
        g_object_unref(colorFillTiles[i]);
    }
}

// ------------------------------------------------------------------------------------------------
// Read image data for the given tileset into the GdkPixbufs tiles array.
void LoadDungeonTiles(void)
{
    GdkPixbuf *sourceLight = NULL;
    GdkPixbuf *sourceDark = NULL;
    GError * error = NULL;

    sourceLight = gdk_pixbuf_new_from_inline(-1, tilesetDungeonCave, FALSE, &error);
    sourceDark = gdk_pixbuf_new_from_inline(-1, tilesetDungeonCaveDark, FALSE, &error);

    for (guint i = 0; i < TILE_COUNT_VP; i++)
    {
        guint pixelX = (i % TILESET_WIDTH) * TILE_SIZE_16;
        guint pixelY = (i / TILESET_WIDTH) * TILE_SIZE_16;

        dungeonLightTiles[i] = gdk_pixbuf_new_subpixbuf(sourceLight, pixelX, pixelY, TILE_SIZE_16, TILE_SIZE_16);
        dungeonDarkTiles[i] = gdk_pixbuf_new_subpixbuf(sourceDark, pixelX, pixelY, TILE_SIZE_16, TILE_SIZE_16);
    }

    g_object_unref(sourceLight);
    g_object_unref(sourceDark);
}

// ------------------------------------------------------------------------------------------------
// Free the GdkPixbufs for the dungeonTiles array.
void FreeDungeonTiles(void)
{
    // Free memory used by GdkPixbufs.
    for (guint i = 0; i < TILE_COUNT_VP; i++)
    {
        g_object_unref(dungeonLightTiles[i]);
        g_object_unref(dungeonDarkTiles[i]);
    }
}

// ------------------------------------------------------------------------------------------------
// Returns a GdkPixbuf based on the given index to the dungeonLightTiles array.
static GdkPixbuf* GetDungeonLightTile(DungeonTile tile)
{
    return dungeonLightTiles[tile];
}

// ------------------------------------------------------------------------------------------------
// Returns a GdkPixbuf based on the given index to the dungeonDarkTiles array.
static GdkPixbuf* GetDungeonDarkTile(DungeonTile tile)
{
    return dungeonDarkTiles[tile];
}

// ------------------------------------------------------------------------------------------------
// Returns the tile image for a TERRAIN_WALL cell based on the surrounding cells.
static DungeonTile GetWallTile(Point *position)
{
    guint tile = TILE_NULL;
    Terrain neighbors[DIR_ALL_COUNT] = {TERRAIN_NULL};

    // Get terrain for each neighboring cell.
    for (guint i = 0; i < DIR_ALL_COUNT; i++)
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
GdkPixbuf* GetTileForActor(Actor *actor)
{
    ActorSpecies species = actor->species;
    ActorTile tile;

    switch (species)
    {
    case SPECIES_PLAYER:
        tile = TILE_PLAYER;
        break;
    case SPECIES_SLIME:
        tile = TILE_SLIME;
        break;
    default:
        tile = TILE_PLAYER;
    }

    return actorTiles[tile];
}

// ------------------------------------------------------------------------------------------------
// Returns the GdkPixbuf from the tiles array for the given cell based on its terrain.
GdkPixbuf* GetTileForTerrain(Point *position)
{
    Terrain terrain = GetCellTerrain(position);
    DungeonTile tile;

    switch (terrain)
    {
    case TERRAIN_FLOOR:
        tile = TILE_FLOOR;
        break;
    case TERRAIN_STAIRS:
        tile = TILE_STAIRS;
        break;
    case TERRAIN_WALL:
    case TERRAIN_NULL:
        tile = GetWallTile(position);
        break;
    default:
        tile = TILE_NULL;
    }

    if (IsVisibleToPlayer(position))
        return GetDungeonLightTile(tile);
    else
        return GetDungeonDarkTile(tile);
}

// ------------------------------------------------------------------------------------------------
// Returns the GdkPixbuf from the tiles array for the cellSelector icon.
GdkPixbuf* GetTileForCellSelector(void)
{
    CellSelectorStatus status = GetSelectedCellStatus();
    DungeonTile tile;

    switch (status)
    {
    case STATUS_UNLOCKED:
        tile = TILE_CELL_SELECTOR_UNLOCKED;
        break;
    case STATUS_LOCKED:
        tile = TILE_CELL_SELECTOR_LOCKED;
        break;
    default:
        tile = TILE_CELL_SELECTOR_UNLOCKED;
    }

    return GetDungeonLightTile(tile);
}

// ------------------------------------------------------------------------------------------------
// Returns the GdkPixbuf from the menuBoxTiles array based on the given UI item.
GdkPixbuf* GetTileForMenuBoxCharacter(CharacterUI item)
{
    switch (item)
    {
    case MB_CHARACTER_TERRAIN_FLIP_BTTN:
        return menuBoxTiles[TILE_REFRESH];
    default:
        return dungeonLightTiles[TILE_NULL];
    }
}

// ------------------------------------------------------------------------------------------------
// Returns the GdkPixbuf from the menuBoxTiles array based on the given UI item.
GdkPixbuf* GetTileForMenuBoxSettings(SettingsUI item)
{
    switch (item)
    {
    case MB_SETTINGS_ZOOM_SWITCH:
        if (GetViewPortZoom() == TRUE)
            return menuBoxTiles[TILE_UI_SWITCH_ON];
        else
            return menuBoxTiles[TILE_UI_SWITCH_OFF];
    case MB_SETTINGS_REFRESH_BUTTON:
        return menuBoxTiles[TILE_REFRESH];
    default:
        return dungeonLightTiles[TILE_NULL];
    }
}

// ------------------------------------------------------------------------------------------------
// Get whether zoom is active on the viewPort.
guint GetTileSizeForZoom(gboolean zoomIsOn)
{
    #ifdef KINDLE_BUILD
    if (zoomIsOn)
        return TILE_SIZE_64;
    else
        return TILE_SIZE_32;
    #else
    if (zoomIsOn)
        return TILE_SIZE_32;
    else
        return TILE_SIZE_16;
    #endif
}

// ------------------------------------------------------------------------------------------------
// Scale the GdkPixbuf tiles based on the viewPort's zoomLevel.
void ScaleTileForZoom(gboolean zoomIsOn)
{
    guint tileSize = GetTileSizeForZoom(zoomIsOn);

    // Scale tiles for dungeon.
    for (guint i = 0; i < TILE_COUNT_VP; i++)
    {
        dungeonLightTiles[i] = gdk_pixbuf_scale_simple(dungeonLightTiles[i], tileSize, tileSize, GDK_INTERP_NEAREST);
        dungeonDarkTiles[i] = gdk_pixbuf_scale_simple(dungeonDarkTiles[i], tileSize, tileSize, GDK_INTERP_NEAREST);
    }

    // Scale tiles for actors.
    for (guint i = 0; i < TILE_COUNT_ACTOR; i++)
    {
        actorTiles[i] = gdk_pixbuf_scale_simple(actorTiles[i], tileSize, tileSize, GDK_INTERP_NEAREST);
    }
}

// ------------------------------------------------------------------------------------------------
// Read image data into the GdkPixbufs menuBoxTiles array.
void LoadMenuBoxTiles(void)
{
    GdkPixbuf *source = NULL;
    GError * error = NULL;

    source = gdk_pixbuf_new_from_inline(-1, tilesetMenuBoxSettings, FALSE, &error);

    for (guint i = 0; i < TILE_COUNT_MB; i++)
    {
        guint pixelX = (i % TILESET_WIDTH) * TILE_SIZE_16;
        guint pixelY = (i / TILESET_WIDTH) * TILE_SIZE_16;

        menuBoxTiles[i] = gdk_pixbuf_new_subpixbuf(source, pixelX, pixelY, TILE_SIZE_16, TILE_SIZE_16);
        menuBoxTiles[i] = gdk_pixbuf_scale_simple(menuBoxTiles[i], TILE_SIZE_BORDER, TILE_SIZE_BORDER,
            GDK_INTERP_NEAREST);
    }

    g_object_unref(source);
}

// ------------------------------------------------------------------------------------------------
// Free the GdkPixbufs for the menuBoxTiles array.
void FreeMenuBoxTiles(void)
{
    // Free memory used by GdkPixbufs.
    for (guint i = 0; i < TILE_COUNT_MB; i++)
    {
        g_object_unref(menuBoxTiles[i]);
    }
}

// ------------------------------------------------------------------------------------------------
// Sets the widget's background color.
void SetWidgetBgColor(GtkWidget *widget, enum Color colorName)
{
    const gchar* string;
    GdkColor color;

    switch (colorName)
    {
    case COLOR_BLACK:
        string = "#000000";
        break;
    case COLOR_GREY_DARK:
        string = "#444444";
        break;
    case COLOR_GREY_LIGHT:
        string = "#bbbbbb";
        break;
    case COLOR_WHITE:
    default:
        string = "#ffffff";
        break;
    }

    if (gdk_color_parse(string, &color))
    {
        gtk_widget_modify_bg(widget, GTK_STATE_NORMAL, &color);
    }
}

// ------------------------------------------------------------------------------------------------
// Draws a rectangle with the given dimensions in the given color.
void FillColorRectangle(cairo_t *context, Point *origin, gint width, gint height, enum Color color)
{
    for (gint y = origin->y; y < origin->y + height; y += TILE_SIZE_16)
    {
        for (gint x = origin->x; x < origin->x + width; x += TILE_SIZE_16)
        {
            gdk_cairo_set_source_pixbuf(context, colorFillTiles[color], x, y);
            cairo_paint(context);
        }
    }
}
