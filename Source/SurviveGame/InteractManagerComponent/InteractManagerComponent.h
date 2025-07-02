// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "SurviveGame/InteractObject/ActorInteractObject.h"
#include "InteractManagerComponent.generated.h"


class USphereComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )

class SURVIVEGAME_API UInteractManagerComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	//RootSkeletalMesh
	UPROPERTY()
	USkeletalMeshComponent* RootSkeletalMesh;
	//InteractObjectDetector
	UPROPERTY(EditAnywhere)
	UBoxComponent* InteractObjectDetector;
	//InteractObjectPointerArrary
	//Store & Sort by distance
	UPROPERTY()
	TArray<AActorInteractObject*> InRangeInteractObjects;
	UPROPERTY()
	TArray<AActorInteractObject*> InteractObjectsSorted;
	UPROPERTY()
	TArray<AActorInteractObject*> tmp_InteractObjectsSorted;
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
