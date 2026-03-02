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

enum Facing
{
    FACING_LEFT,
    FACING_RIGHT
};

typedef struct
{
    ActorSpecies species;
    Point position;
    Point prevPosition;
    guint facing:1;
    gint healthCurrent;
    gint damagedThisTurn:1;
    guint sightRange:4;
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
guint GetActorFacing(Actor *actor);
void SetActorFacing(Actor *actor, guint facing);
void UpdateActorFacing(Actor *actor, Direction direction);
gint GetActorHealthCurrent(Actor *actor);
void SetActorHealthCurrent(Actor *actor, gint newHealth);
gboolean IsActorDead(Actor *actor);
gboolean ReincarnateActor(Actor *actor, ActorSpecies species);
void ReincarnateAllActors(void);
gint HasActorBeenDamaged(Actor *actor);
void SetActorDamaged(Actor *actor, gboolean boolean);
guint GetActorSightRange(Actor *actor);
void SetActorSightRange(Actor *actor, guint sightRange);

#endif // ACTOR_H
