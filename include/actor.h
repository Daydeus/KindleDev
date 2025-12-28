#ifndef ACTOR_H
#define ACTOR_H

#include <gtk-2.0/gtk/gtk.h>
#include <glib-2.0/glib.h>
#include "global.h"

// ------------------------------------------------------------------------------------------------
// Project Defines
// ------------------------------------------------------------------------------------------------

#define MAX_ACTOR_COUNT    5
#define PLAYER_ACTOR_INDEX 0

// ------------------------------------------------------------------------------------------------
// Data Types
// ------------------------------------------------------------------------------------------------

enum ActorSpecies
{
    SPECIES_NULL,
    SPECIES_PLAYER,
    SPECIES_SLIME,
    SPECIES_COUNT
};

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

void InitActors(void);
void PlaceAllActors(void);
Actor* GetActor(guint actorIndex);
ActorSpecies GetActorSpecies(Actor *actor);
void SetActorSpecies(Actor *actor, ActorSpecies species);
Point* GetActorPosition(Actor *actor);
void SetActorPosition(Actor *actor, Point *position);

#endif // ACTOR_H
