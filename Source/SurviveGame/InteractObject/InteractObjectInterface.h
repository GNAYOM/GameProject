// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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
	FString InteractObjectNameStatus;
	UPROPERTY(EditAnywhere)
	UDataTable* InteractObjectDataTable;
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
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void UpdateInteractObjectStatus(FString NewInteractObjectNameStatus);
};
