#ifndef ACTOR_H
#define ACTOR_H

#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------

#define MAX_ACTOR_COUNT 1

// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------

enum Direction
{
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT,
    DIR_COUNT
};

enum ActorSpecies
{
    SPECIES_NULL,
    SPECIES_PLAYER,
    SPECIES_COUNT
};

typedef struct
{
    gint x;
    gint y;
} Point;

typedef struct
{
    ActorSpecies species;
    Point position;
} Actor;

// ------------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------------

extern Actor actors[MAX_ACTOR_COUNT];

// ------------------------------------------------------------------------------------------------
// Function Declarations
// ------------------------------------------------------------------------------------------------

Actor* GetActor(guint actorIndex);
ActorSpecies GetActorSpecies(Actor *actor);
void SetActorSpecies(Actor *actor, ActorSpecies species);
Point* GetActorPosition(Actor *actor);
void SetActorPosition(Actor *actor, gint positionX, gint positionY);
void MoveActorPosition(Actor *actor, Direction direction, guint distance);

#endif // ACTOR_H
