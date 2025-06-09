// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "SurviveGame/GameEvent/GameEvent.h"
#include "SurviveGame/InteractObject/InteractObjectInterface.h"
#include "SurviveGame/Enum/InputEnum/InputEnum.h"
#include "InteractObject.generated.h"
UCLASS()
class SURVIVEGAME_API AInteractObject : public AInteractObjectInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AInteractObject();
	
	//EventArray
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//InteractObjectVolume
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UBoxComponent* InteractObjectVolume;
	//BlockWhenCameraOverlap
	UPROPERTY()
	UPrimitiveComponent* PrimitiveComponent;

	//Detected
	UFUNCTION()
	void OnCamBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnCamEndOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex); 
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//EventRelease
	void ReleaseEventActively(int OptionSlotNum);
	//InteractObjectStatusChanged
	void UpdateItemStatus(FString NewInteractObjectStatus) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
