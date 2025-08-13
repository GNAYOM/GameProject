// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SurviveGame/InteractObject/InteractObjectProperty.h"
#include "SurviveGame/InteractObject/InteractObjectSocket.h"
#include "SurviveGame/InteractObject/InteractObjectPlug.h"
#include "SurviveGame/InteractObject/EquipmentAbility/EquipmentAbilityInterface.h"
#include "SurviveGame/Enum/InteractObjectStatus/InteractObjectStatus.h"
#include "UObject/ReferenceChainSearch.h"
#include "ActorInteractObjectInterface.generated.h"

struct TargetSocketInfo
{
	bool BeginMerge;
	int SocketRowIndex;
	int SocketColIndex;
};
UCLASS(Abstract)
class SURVIVEGAME_API AActorInteractObjectInterface : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AActorInteractObjectInterface();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	
	UPROPERTY()
	UPrimitiveComponent* PrimitiveComponent;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
	FString BehaviorStatus;
	UPROPERTY(EditAnywhere)
	FString InitialProperties;
	UPROPERTY(EditAnywhere)
	FString CurrentScript;
	UPROPERTY(EditAnywhere)
	FString ScriptPath;
	UPROPERTY(EditAnywhere)
	FString Script;
	UPROPERTY(EditAnywhere)
	UDataTable* BehaviorDataTable;
	UPROPERTY(EditAnywhere)
	UDataTable* PropertiesDataTable;
	UPROPERTY(EditAnywhere)
	UDataTable* ScriptDataTable;
	//InteractOption
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool IsSelectedByPlayer;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText	Option1;
	UPROPERTY(EditAnywhere)
	int EventID1;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText	Option2;
	UPROPERTY(EditAnywhere)
	int EventID2;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText	Option3;
	UPROPERTY(EditAnywhere)
	int EventID3;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText	Option4;
	UPROPERTY(EditAnywhere)
	int EventID4;
	UPROPERTY(EditAnywhere)
	int EVENTIDAutoRelease;
	//Socket
	SocketPanel PossessedSocketPanel;
	//Plug
	Plug Plug;
	TargetSocketInfo CurrentSocketInfo;
	//Properties
	InteractObjectProperty Property;
	UPROPERTY(EditAnywhere)
	float Weight;
	//InteractObjectStatus
	int InteractObjectStatus = Default;
	//ForKeyDetection
	bool Option1JustPressed = false;
	bool Option2JustPressed = false;
	bool Option3JustPressed = false;
	bool Option4JustPressed = false;
	//CustomInput
	bool CustomInput1 = false;
	bool CustomInput2 = false;
	
	// Called to bind functionality to input
	void UpdateBehaviorStatus(FString NewItemStatus);
	void UpdateInitialProperties(FString NewScript);
	void UpdateCurrentScript(FString NewScript);
	virtual void AutoReleaseEvent() ;
	virtual void ReleaseEventActively(int Input) ;
	//Get/Set Status
	virtual FVector GetPlayerBackSocketPosition();
	virtual FRotator GetPlayerDirectionRotator();
	virtual FVector GetPlayerRHandSocketPosition();
	virtual UStaticMeshComponent* GetPlayerBackSocketComponent();
	//Set InteractObjectSystem
	virtual void SetPlayerPossessedInteractObjectSystem();
	virtual bool MergeWithPlayerPossessedInteractObjectSystem();
	virtual void MergeWithSourceInteractObject();
	virtual void SeperateFromPlayerPossessedInteractObjectSystem();
	virtual void SeperateFromSourceInteractObject();
	virtual void SetNewInteractObjectSystem();
	virtual void SetConnectionWithInteractObject();
	//PlayerEquip
	virtual void SetAsPlayerBlockingEquippedInteractObject();
	virtual void ClearPlayerBlockingEquippedInteractObject();
	//EquipmentAbility
	UPROPERTY()
	UEquipmentAbilityInterface* EquipmentAbility;
	virtual void UseEquipment();//WIP
	//InitSocket
	virtual void InitSocketPanel();
	//CameraCollosion
	bool IgnoreCamera = false;
	//SourceInteractObject
	UPROPERTY()
	AActorInteractObjectInterface* SourceInteractObject;
	//Test
	virtual void LogHello();
	
};