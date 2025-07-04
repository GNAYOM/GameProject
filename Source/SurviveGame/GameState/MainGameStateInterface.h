#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
//#include "SurviveGame/InteractObject/ActorInteractObject.h"
#include "InteractObjectSystem.h"
#include "Experimental/Graph/GraphConvert.h"
#include "MainGameStateInterface.generated.h"


/**
 * 
 */
UCLASS(Abstract)
class SURVIVEGAME_API AMainGameStateInterface : public AGameStateBase
{
	GENERATED_BODY()
	virtual InteractObjectSystem* CreateInteractObjectSystem(AActorInteractObjectInterface* FirstInteractObject);
	virtual InteractObjectSystem* MergeInteractObjectSystem();
	virtual InteractObjectSystem* SeperateInteractObjectSystem();
	virtual InteractObjectSystem* DestroyInteractObjectSystem();

};