// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractManagerComponentInterface.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "SurviveGame/InteractObject/ActorInteractObjectInterface.h"
#include "SurviveGame/Enum/InteractManagerComponentStatusEnum/InteractManagerComponentStatusEnum.h"
#include "SurviveGame/PlayerState/MainPlayerState.h"
#include "InteractManagerComponent.generated.h"


class USphereComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )

class SURVIVEGAME_API UInteractManagerComponent : public UInteractManagerComponentInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractManagerComponent();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	//PlayerStatus
	UPROPERTY()
	AMainPlayerState* MainPlayerState;
	//RootSkeletalMesh
	UPROPERTY()
	USkeletalMeshComponent* RootSkeletalMesh;
	//InteractObjectDetector
	UPROPERTY(EditAnywhere)
	UBoxComponent* InteractObjectDetector;
	//InteractObjectPointerArrary
	//Store & Sort by distance
	UPROPERTY()
	TArray<AActorInteractObjectInterface*> InRangeInteractObjects;
	UPROPERTY()
	TArray<AActorInteractObjectInterface*> InteractObjectsSorted;
	UPROPERTY()
	TArray<AActorInteractObjectInterface*> tmp_InteractObjectsSorted;
	//Selected InteractObject
	UPROPERTY()
	AActorInteractObjectInterface* LastSelectedTargetInteractObject;
	UPROPERTY()
	AActorInteractObjectInterface* SelectedTargetInteractObject;
	void InteractObjectSelection();
	void SelectInteractObjectByRange();
	void SelectInteractObjectFromBackSocket();
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void InteractObjectInputDetected(int Input);
	
	UFUNCTION()
	void OnInteractObjectBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnInteractObjectEndOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
		
};
