#pragma once

#include "SurviveGame/InteractObject/ActorInteractObjectInterface.h"
/**
 * 
 */
struct InteractObjectSystem
{
	TArray<TArray<int>>	ConnectionMatrix;
	TArray<AActorInteractObjectInterface*> TotalInteractObjects;
	InteractObjectSystem* NewInteractObjectSystem;
	bool IsPossessedByPlayer;
	//TArray<AActorInteractObjectInterface*> BackStorage;
};
