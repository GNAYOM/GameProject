// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "SurviveGame/GameEvent/GameEvent.h"
#include "ActorInteractObjectInterface.h"
#include "Components/CapsuleComponent.h"
#include "SurviveGame/Enum/InputEnum/InputEnum.h"
#include "SurviveGame/GameState/MainGameState.h"
#include "SurviveGame/PlayerState/MainPlayerState.h"
#include "SurviveGame/GameState/InteractObjectSystem.h"
#include "SurviveGame/InteractManagerComponent/InteractManagerComponentInterface.h"
#include "SurviveGame/InteractObject/EquipmentAbility/TestAbility/TestEquipmentAbility.h"
#include "ActorInteractObject.generated.h"

UCLASS()
class SURVIVEGAME_API AActorInteractObject : public AActorInteractObjectInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AActorInteractObject();
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* MeshComponent;
	//InteractObjectSystem
	InteractObjectSystem* CurrentInteractObjectSystem;
	//GameState
	UPROPERTY()
	AMainGameState* MainGameState;
	UPROPERTY()
	AMainPlayerState* MainPlayerState;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//InteractObjectVolume
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UBoxComponent* InteractObjectVolume;
	//BlockWhenCameraOverlap
	UPROPERTY(EditAnywhere)
	UCapsuleComponent* Collision;
	
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
	void AutoReleaseEvent() override;
	void ReleaseEventActively(int Input) override;
	UPROPERTY(EditAnywhere)
	UPhysicsConstraintComponent* A;
	//Get/Set PlayerState
	FVector GetPlayerBackSocketPosition() override;
	FVector GetPlayerRHandSocketPosition() override;
	FRotator GetPlayerDirectionRotator() override;
	UStaticMeshComponent* GetPlayerBackSocketComponent() override;
	//Set InteractObjectSystem
	virtual void SetPlayerPossessedInteractObjectSystem() override;
	virtual bool MergeWithPlayerPossessedInteractObjectSystem() override;
	virtual bool MergeWithTargetInteractObject() override;
	virtual void MergeWithSourceInteractObject() override;
	virtual void SeperateFromPlayerPossessedInteractObjectSystem() override;
	virtual bool SeperateWithTargetInteractObject() override;
	virtual void SeperateFromSourceInteractObject() override;
	virtual void SetNewInteractObjectSystem() override;
	//PlayerEquip
	virtual void SetAsPlayerBlockingEquippedInteractObject() override;
	virtual void ClearPlayerBlockingEquippedInteractObject() override;
	//EquipmentAbility
	virtual void UseEquipment() override;
	//Socket
	virtual void InitSocketPanel() override;
	TargetSocketInfo FindTargetSocket(SocketPanel* TargetSocketPanel);
	void SetSocketsOccupied(int RowIndex,int ColIndex);
	void DisconnectFromSocket(int RowIndex,int ColIndex);
	//Test
	virtual void LogHello() override;
};
