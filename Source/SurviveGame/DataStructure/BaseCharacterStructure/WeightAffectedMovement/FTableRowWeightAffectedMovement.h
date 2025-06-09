#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FTableRowWeightAffectedMovement.generated.h"

USTRUCT(BlueprintType)
struct  FTableRowWeightAffectedMovement :public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	FTableRowWeightAffectedMovement();
	UPROPERTY(EditAnywhere,BlueprintReadOnly,DisplayName = "GroundFriction")
	float GroundFriction;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,DisplayName = "BrakingFrictionFactor")
	float BrakingFrictionFactor;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,DisplayName = "MaxAccelerate")
	float MaxAcceleration;
	~FTableRowWeightAffectedMovement();
	
};
