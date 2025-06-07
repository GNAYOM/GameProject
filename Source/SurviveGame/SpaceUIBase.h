// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SurviveGame/GameCharacter/BaseCharacter/BaseCharacter.h"
#include "SpaceUIBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURVIVEGAME_API USpaceUIBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpaceUIBase();
	//MainCameraWorldLocation
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FVector MainCameraLocation;
	UFUNCTION(BlueprintCallable)
	FVector MainCameraWorldLocation();
	UPROPERTY()
	ABaseCharacter* MainCharacter;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
