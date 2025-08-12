#pragma once
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SurviveGame/InteractObject/EquipmentAbility/EquipmentAbilityInterface.h"
#include "TestEquipmentAbility.generated.h"

UCLASS()
class SURVIVEGAME_API UTestEquipmentAbility : public UEquipmentAbilityInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTestEquipmentAbility();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void UseEquipment();
		
};
