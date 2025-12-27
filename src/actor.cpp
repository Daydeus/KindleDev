#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include <cstdlib>
#include "global.h"
#include "actor.h"
#include "dungeonCell.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

Actor actors[MAX_ACTOR_COUNT] = {};

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Initialize non-position data for all actors.
void InitActors(void)
{
    for (guint index = 0; index < MAX_ACTOR_COUNT; index++)
    {
        Actor *actor = GetActor(index);

        if (index == 0)
            SetActorSpecies(actor, SPECIES_PLAYER);
        else
            SetActorSpecies(actor, SPECIES_SLIME);
    }
}

// ------------------------------------------------------------------------------------------------
// Place all actors on valid cells within the dungeon.
void PlaceAllActors(void)
{
    for (guint index = 0; index < MAX_ACTOR_COUNT; index++)
    {
        Actor *actor = GetActor(index);
        Point position = {0, 0};

        while (GetCellTerrain(&position) != TERRAIN_FLOOR)
        {
            position.x = rand() % DUNGEON_WIDTH;
            position.y = rand() % DUNGEON_HEIGHT;
        }

        SetActorPosition(actor, &position);
    }
}

// ------------------------------------------------------------------------------------------------
// Returns the actor at the given index of the actors array.
Actor* GetActor(guint actorIndex)
{
    return &actors[actorIndex];
}

// ------------------------------------------------------------------------------------------------
// Gets the species of the given actor.
ActorSpecies GetActorSpecies(Actor *actor)
{
    return actor->species;
}

// ------------------------------------------------------------------------------------------------
// Sets the species of the given actor.
void SetActorSpecies(Actor *actor, ActorSpecies species)
{
    actor->species = species;
}

// ------------------------------------------------------------------------------------------------
// Gets the position of the given actor.
Point* GetActorPosition(Actor *actor)
{
    return &actor->position;
}

// ------------------------------------------------------------------------------------------------
// Sets the position of the given actor.
void SetActorPosition(Actor *actor, Point* position)
{
    if (IsOutsideDungeon(position))
        return;

    Point *oldPosition = GetActorPosition(actor);

    SetCellsActor(oldPosition, NULL);
    SetCellsActor(position, actor);

    actor->position = *position;
}
