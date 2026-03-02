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

        if (index == PLAYER_ACTOR_INDEX)
        {
            SetActorSpecies(actor, SPECIES_PLAYER);
            SetActorHealthCurrent(actor, 5);
            SetActorSightRange(actor, 6);
        }
        else
        {
            SetActorSpecies(actor, SPECIES_SLIME);
            SetActorHealthCurrent(actor, 2);
            SetActorSightRange(actor, 5);
        }

        SetActorFacing(actor, rand() % 2);
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

        while (GetCellTerrain(&position) != TERRAIN_FLOOR || IsCellOccupiedByActor(&position))
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

    actor->prevPosition = *GetActorPosition(actor);

    SetCellsActor(&actor->prevPosition, NULL);
    SetCellsActor(position, actor);

    actor->position = *position;
}

// ------------------------------------------------------------------------------------------------
// Gets the facing direction of the given actor.
guint GetActorFacing(Actor *actor)
{
    return actor->facing;
}

// ------------------------------------------------------------------------------------------------
// Sets the facing direction of the given actor.
void SetActorFacing(Actor *actor, guint facing)
{
    actor->facing = facing;
}

// ------------------------------------------------------------------------------------------------
// Updates the facing direction of the given actor based on movement direction.
void UpdateActorFacing(Actor *actor, Direction direction)
{
    switch (direction)
    {
    case DIR_NORTH_EAST:
    case DIR_EAST:
    case DIR_SOUTH_EAST:
        SetActorFacing(actor, FACING_RIGHT);
        break;
    case DIR_SOUTH_WEST:
    case DIR_WEST:
    case DIR_NORTH_WEST:
        SetActorFacing(actor, FACING_LEFT);
        break;
    default:
        break;
    }
}

// ------------------------------------------------------------------------------------------------
// Gets the current health for the given actor.
gint GetActorHealthCurrent(Actor *actor)
{
    return actor->healthCurrent;
}

// ------------------------------------------------------------------------------------------------
// Sets the current helath for the given actor.
void SetActorHealthCurrent(Actor *actor, gint newHealth)
{
    actor->healthCurrent = newHealth;
}

// ------------------------------------------------------------------------------------------------
// Returns TRUE is the given actor's health is less than or equal to zero.
gboolean IsActorDead(Actor *actor)
{
    if (actor->healthCurrent > 0)
        return FALSE;
    else
        return TRUE;
}

// ------------------------------------------------------------------------------------------------
// If the given actor is dead, respawns them as the given species.
gboolean ReincarnateActor(Actor *actor, ActorSpecies species)
{
    if (IsActorDead(actor))
    {
        SetActorSpecies(actor, species);
        SetActorHealthCurrent(actor, 2);
        SetActorSightRange(actor, 3);

        return TRUE;
    }

    return FALSE;
}

// ------------------------------------------------------------------------------------------------
// Loops through all actors and reincarnates the dead ones.
void ReincarnateAllActors(void)
{
    for (guint i = 0; i < MAX_ACTOR_COUNT; i++)
    {
        Actor *actor = GetActor(i);
        ActorSpecies species = SPECIES_SLIME;

        ReincarnateActor(actor, species);
    }
}

// ------------------------------------------------------------------------------------------------
// Returns TRUE if the actor has been damaged this turn.
gint HasActorBeenDamaged(Actor *actor)
{
    return actor->damagedThisTurn;
}

// ------------------------------------------------------------------------------------------------
// Sets if the actor has been damaged this turn.
void SetActorDamaged(Actor *actor, gboolean boolean)
{
    actor->damagedThisTurn = boolean;
}

// ------------------------------------------------------------------------------------------------
// Gets the sight range for the given actor based on their species.
guint GetActorSightRange(Actor *actor)
{
    return actor->sightRange;
}

// ------------------------------------------------------------------------------------------------
// Sets the sight range of the given actor based on their species.
void SetActorSightRange(Actor *actor, guint sightRange)
{
    actor->sightRange = sightRange;
}
