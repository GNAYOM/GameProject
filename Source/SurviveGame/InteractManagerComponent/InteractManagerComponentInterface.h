#pragma once

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "SurviveGame/InteractObject/ActorInteractObjectInterface.h"
#include "SurviveGame/Enum/InteractManagerComponentStatusEnum/InteractManagerComponentStatusEnum.h"
#include "InteractManagerComponentInterface.generated.h"


class USphereComponent;

UCLASS(Abstract)

class SURVIVEGAME_API UInteractManagerComponentInterface : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractManagerComponentInterface();
	//InteractManagerComponent Status
	int InteractManagerComponentStatus;
	//EquippedInteractObject
	UPROPERTY()
	AActorInteractObjectInterface* EquippedInteractObject;
};