#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
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
    DungeonCell *oldCell = GetCellAtPosition(oldPosition->x, oldPosition->y);
    DungeonCell *newCell = GetCellAtPosition(positionX, positionY);

    actor->position.x = positionX;
    actor->position.y = positionY;
    oldCell->actor = NULL;
    newCell->actor = actor;

    g_print("Actor's position: (%d, %d).\n", actors[0].position.x, actors[0].position.y);
}

// ------------------------------------------------------------------------------------------------
// Moves the dungeonCell position of the given actor based on the given direction and distance.
void MoveActorPosition(Actor *actor, Direction direction, guint distance)
{
    Point *position = GetActorPosition(actor);

    switch (direction)
    {
    case DIR_UP:
        position->y -= distance;
        break;
    case DIR_DOWN:
        position->y += distance;
        break;
    case DIR_LEFT:
        position->x -= distance;
        break;
    case DIR_RIGHT:
        position->x += distance;
        break;
    default:
        break;
    }

    SetActorPosition(actor, position->x, position->y);
}

