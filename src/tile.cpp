#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include "actor.h"
#include "dungeonCell.h"
#include "fieldOfView.h"
#include "global.h"
#include "menu.h"
#include "menuLayout.h"
#include "tile.h"
#include "viewPort.h"
#include "data/tilesetActor.h"
#include "data/tilesetBorder.h"
#include "data/tilesetColorFill.h"
#include "data/tilesetMenu.h"
#include "data/tilesetTerrainDark.h"
#include "data/tilesetTerrainLight.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

GdkPixbuf *actorTiles[TILE_ACTOR_COUNT] = {NULL};
GdkPixbuf *borderTiles[TILE_BORDER_COUNT] = {NULL};
GdkPixbuf *colorFillTiles[COLOR_COUNT_ALL] = {NULL};
GdkPixbuf *menuTiles[TILE_MENU_COUNT] = {NULL};
GdkPixbuf *terrainDarkTiles[TILE_TERRAIN_COUNT] = {NULL};
GdkPixbuf *terrainLightTiles[TILE_TERRAIN_COUNT] = {NULL};

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

static GdkPixbuf* GetTerrainDarkTile(TerrainTile tile);
static GdkPixbuf* GetTerrainLightTile(TerrainTile tile);
static TerrainTile GetWallTile(Point *position);

// ------------------------------------------------------------------------------------------------
// Read image data into the GdkPixbufs actorTiles array.
void LoadActorTiles(void)
{
    GdkPixbuf *source = NULL;
    GError * error = NULL;

    source = gdk_pixbuf_new_from_inline(-1, tilesetActor, FALSE, &error);

    for (guint i = 0; i < TILE_ACTOR_COUNT; i++)
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
    for (guint i = 0; i < TILE_ACTOR_COUNT; i++)
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

    for (guint i = 0; i < TILE_BORDER_COUNT; i++)
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
    for (guint i = 0; i < TILE_BORDER_COUNT; i++)
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
void LoadTerrainTiles(void)
{
    GdkPixbuf *sourceDark = NULL;
    GdkPixbuf *sourceLight = NULL;
    GError * error = NULL;

    sourceLight = gdk_pixbuf_new_from_inline(-1, tilesetTerrainLight, FALSE, &error);
    sourceDark = gdk_pixbuf_new_from_inline(-1, tilesetTerrainDark, FALSE, &error);

    for (guint i = 0; i < TILE_TERRAIN_COUNT; i++)
    {
        guint pixelX = (i % TILESET_WIDTH) * TILE_SIZE_16;
        guint pixelY = (i / TILESET_WIDTH) * TILE_SIZE_16;

        terrainDarkTiles[i] = gdk_pixbuf_new_subpixbuf(sourceDark, pixelX, pixelY, TILE_SIZE_16, TILE_SIZE_16);
        terrainLightTiles[i] = gdk_pixbuf_new_subpixbuf(sourceLight, pixelX, pixelY, TILE_SIZE_16, TILE_SIZE_16);
    }

    g_object_unref(sourceDark);
    g_object_unref(sourceLight);
}

// ------------------------------------------------------------------------------------------------
// Free the GdkPixbufs for the dungeonTiles array.
void FreeTerrainTiles(void)
{
    // Free memory used by GdkPixbufs.
    for (guint i = 0; i < TILE_TERRAIN_COUNT; i++)
    {
        g_object_unref(terrainDarkTiles[i]);
        g_object_unref(terrainLightTiles[i]);
    }
}

// ------------------------------------------------------------------------------------------------
// Returns a GdkPixbuf based on the given index to the dungeonLightTiles array.
static GdkPixbuf* GetTerrainLightTile(TerrainTile tile)
{
    return terrainLightTiles[tile];
}

// ------------------------------------------------------------------------------------------------
// Returns a GdkPixbuf based on the given index to the dungeonDarkTiles array.
static GdkPixbuf* GetTerrainDarkTile(TerrainTile tile)
{
    return terrainDarkTiles[tile];
}

// ------------------------------------------------------------------------------------------------
// Returns the tile image for a TERRAIN_WALL cell based on the surrounding cells.
static TerrainTile GetWallTile(Point *position)
{
    guint tile = TILE_TERRAIN_EDGE;
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

    return (TerrainTile)tile;
}

// ------------------------------------------------------------------------------------------------
// Returns the GdkPixbuf from the tiles array for the given actor.
GdkPixbuf* GetTileForActor(Actor *actor)
{
    ActorSpecies species = actor->species;
    guint facing = GetActorFacing(actor);
    ActorTile tile;

    switch (species)
    {
    case SPECIES_PLAYER:
        tile = (facing == FACING_LEFT) ? TILE_ACTOR_PLAYER_LEFT : TILE_ACTOR_PLAYER_RIGHT;
        break;
    case SPECIES_SLIME:
        tile = (facing == FACING_LEFT) ? TILE_ACTOR_SLIME_LEFT : TILE_ACTOR_SLIME_RIGHT;
        break;
    default:
        tile = TILE_ACTOR_DUMMY;
    }

    return actorTiles[tile];
}

// ------------------------------------------------------------------------------------------------
// Returns the GdkPixbuf from the tiles array for the given cell based on its terrain.
GdkPixbuf* GetTileForTerrain(Point *position)
{
    Terrain terrain = GetCellTerrain(position);
    TerrainTile tile;

    switch (terrain)
    {
    case TERRAIN_FLOOR:
        tile = TILE_TERRAIN_FLOOR;
        break;
    case TERRAIN_STAIRS:
        tile = TILE_TERRAIN_STAIRS;
        break;
    case TERRAIN_WALL:
        tile = GetWallTile(position);
        break;
    default:
        tile = TILE_TERRAIN_EDGE;
    }

    if (IsVisibleToPlayer(position))
        return GetTerrainLightTile(tile);
    else
        return GetTerrainDarkTile(tile);
}

// ------------------------------------------------------------------------------------------------
// Returns the GdkPixbuf from the actorTiles array for the cellSelector icon.
GdkPixbuf* GetTileForCellSelector(void)
{
    CellSelectorStatus status = GetSelectedCellStatus();

    switch (status)
    {
    case STATUS_LOCKED:
        return actorTiles[TILE_ACTOR_SELECTOR_LOCKED];
    case STATUS_UNLOCKED:
    default:
        return actorTiles[TILE_ACTOR_SELECTOR_UNLOCKED];
    }
}

// ------------------------------------------------------------------------------------------------
// Returns the GdkPixbuf from the menuTiles array for the given state.
GdkPixbuf* GetTileForMenuState(MenuState state)
{
    MenuState currentState = GetMenuState();

    switch (state)
    {
    case STATE_INSPECT:
        if (currentState == STATE_INSPECT)
            return menuTiles[TILE_MENU_INSPECT_ON];
        else
            return menuTiles[TILE_MENU_INSPECT_OFF];
    case STATE_CHARACTER:
        if (currentState == STATE_CHARACTER)
            return menuTiles[TILE_MENU_CHARACTER_ON];
        else
            return menuTiles[TILE_MENU_CHARACTER_OFF];
    case STATE_INVENTORY:
        if (currentState == STATE_INVENTORY)
            return menuTiles[TILE_MENU_INVENTORY_ON];
        else
            return menuTiles[TILE_MENU_INVENTORY_OFF];
    case STATE_LOGBOOK:
        if (currentState == STATE_LOGBOOK)
            return menuTiles[TILE_MENU_LOGBOOK_ON];
        else
            return menuTiles[TILE_MENU_LOGBOOK_OFF];
    case STATE_SETTINGS:
        if (currentState == STATE_SETTINGS)
            return menuTiles[TILE_MENU_SETTINGS_ON];
        else
            return menuTiles[TILE_MENU_SETTINGS_OFF];
    default:
        return menuTiles[0];
    }
}

// ------------------------------------------------------------------------------------------------
// Returns the GdkPixbuf from the menuTiles array based on the given UI item.
GdkPixbuf* GetTileForMenuCharacter(CharacterUI item)
{
    switch (item)
    {
    case MB_CHARACTER_TERRAIN_FLIP_BTTN:
        return menuTiles[TILE_MENU_REFRESH];
    default:
        return terrainLightTiles[TILE_TERRAIN_EDGE];
    }
}

// ------------------------------------------------------------------------------------------------
// Returns the GdkPixbuf from the menuTiles array based on the given UI item.
GdkPixbuf* GetTileForMenuSettings(SettingsUI item)
{
    switch (item)
    {
    case MB_SETTINGS_ZOOM_SWITCH:
        if (GetViewPortZoom() == TRUE)
            return menuTiles[TILE_MENU_UI_SWITCH_ON];
        else
            return menuTiles[TILE_MENU_UI_SWITCH_OFF];
    case MB_SETTINGS_REFRESH_BUTTON:
        return menuTiles[TILE_MENU_REFRESH];
    case MB_SETTINGS_FOGOFWAR_BUTTON:
        if (GetFogOfWarStatus() == TRUE)
            return menuTiles[TILE_MENU_UI_SWITCH_ON];
        else
            return menuTiles[TILE_MENU_UI_SWITCH_OFF];
    case MB_SETTINGS_EXIT_BUTTON:
        return menuTiles[TILE_MENU_EXIT];
    default:
        return terrainLightTiles[TILE_TERRAIN_EDGE];
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
    for (guint i = 0; i < TILE_TERRAIN_COUNT; i++)
    {
        terrainDarkTiles[i] = gdk_pixbuf_scale_simple(terrainDarkTiles[i], tileSize, tileSize, GDK_INTERP_NEAREST);
        terrainLightTiles[i] = gdk_pixbuf_scale_simple(terrainLightTiles[i], tileSize, tileSize, GDK_INTERP_NEAREST);
    }

    // Scale tiles for actors.
    for (guint i = 0; i < TILE_ACTOR_COUNT; i++)
    {
        actorTiles[i] = gdk_pixbuf_scale_simple(actorTiles[i], tileSize, tileSize, GDK_INTERP_NEAREST);
    }
}

// ------------------------------------------------------------------------------------------------
// Read image data into the GdkPixbufs menuTiles array.
void LoadMenuTiles(void)
{
    GdkPixbuf *source = NULL;
    GError * error = NULL;

    source = gdk_pixbuf_new_from_inline(-1, tilesetMenu, FALSE, &error);

    for (guint i = 0; i < TILE_MENU_COUNT; i++)
    {
        guint pixelX = (i % TILESET_WIDTH) * TILE_SIZE_16;
        guint pixelY = (i / TILESET_WIDTH) * TILE_SIZE_16;

        menuTiles[i] = gdk_pixbuf_new_subpixbuf(source, pixelX, pixelY, TILE_SIZE_16, TILE_SIZE_16);
        menuTiles[i] = gdk_pixbuf_scale_simple(menuTiles[i], TILE_SIZE_MB, TILE_SIZE_MB,
            GDK_INTERP_NEAREST);
    }

    g_object_unref(source);
}

// ------------------------------------------------------------------------------------------------
// Free the GdkPixbufs for the menuTiles array.
void FreeMenuTiles(void)
{
    // Free memory used by GdkPixbufs.
    for (guint i = 0; i < TILE_MENU_COUNT; i++)
    {
        g_object_unref(menuTiles[i]);
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
