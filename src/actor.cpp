#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
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
void SetActorPosition(Actor *actor, gint positionX, gint positionY)
{
    if (IsOutsideDungeon(positionX, positionY))
        return;

    Point *oldPosition = GetActorPosition(actor);

    SetCellsActor(oldPosition->x, oldPosition->y, NULL);
    SetCellsActor(positionX, positionY, actor);

    actor->position.x = positionX;
    actor->position.y = positionY;
}

// ------------------------------------------------------------------------------------------------
// Moves the dungeonCell position of the given actor based on the given direction and distance.
void MoveActorPosition(Actor *actor, Direction direction, guint distance)
{
    Point *position = GetActorPosition(actor);

    SetCellsActor(position->x, position->y, NULL);

    position->x += hMovement[direction] * distance;
    position->y += vMovement[direction] * distance;

    SetActorPosition(actor, position->x, position->y);
    SetCellsActor(position->x, position->y, actor);
}

