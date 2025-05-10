// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "InteractObject.generated.h"
UCLASS()
class SURVIVEGAME_API AInteractObject : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AInteractObject();
	//InteractOption
	UPROPERTY(EditAnywhere)
	FString InteractObjectName;
	UPROPERTY(EditAnywhere)
	UDataTable* InteractObjectDataTable;
	UPROPERTY(EditAnywhere)
	FText	Option1;
	UPROPERTY(EditAnywhere)
	FText	Option2;
	UPROPERTY(EditAnywhere)
	FText	Option3;
	UPROPERTY(EditAnywhere)
	FText	Option4;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//InteractObjectVolume
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UBoxComponent* InteractObjectVolume;
	//BlockWhenCameraOverlap
	UPROPERTY()
	UPrimitiveComponent* PrimitiveComponent;
	UFUNCTION()
	void OnCamBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnCamEndOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex); 
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
