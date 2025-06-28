// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SurviveGame/InteractObject/InteractObjectProperty.h"
#include "InteractObjectInterface.generated.h"

UCLASS(Abstract)
class SURVIVEGAME_API AInteractObjectInterface : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AInteractObjectInterface();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
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
	//Properties
	InteractObjectProperty Property;
	UPROPERTY(EditAnywhere)
	float Weight;
	//ForKeyDetection
	bool Option1JustPressed = false;
	bool Option2JustPressed = false;
	bool Option3JustPressed = false;
	bool Option4JustPressed = false;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void UpdateBehaviorStatus(FString NewItemStatus);
	virtual void UpdateInitialProperties(FString NewScript);
	virtual void UpdateCurrentScript(FString NewScript);
};
