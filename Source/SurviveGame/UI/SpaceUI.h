// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SurviveGame/GameCharacter/TestCharacter/TestCharacter.h"
#include "SpaceUI.generated.h"

UCLASS()
class SURVIVEGAME_API ASpaceUI : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpaceUI();
	//MainCameraWorldLocation
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FVector MainCameraLocation;
	UFUNCTION(BlueprintCallable)
	FVector MainCameraWorldLocation();
	UPROPERTY()
	ATestCharacter* MainCharacter;
	//RotationToFrontCaculate
	UFUNCTION(BlueprintCallable)
	float RightAngleYawRotate(float YawRotation,float AbsAllowableRange );
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
