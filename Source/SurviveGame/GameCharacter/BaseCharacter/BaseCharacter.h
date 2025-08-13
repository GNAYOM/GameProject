// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "SurviveGame/InteractManagerComponent/InteractManagerComponent.h"
#include "SurviveGame/GameState/MainGameState.h"
#include "SurviveGame/PlayerState/MainPlayerState.h"
#include "SurviveGame/Enum/InputEnum/InputEnum.h"
#include "BaseCharacter.generated.h"

UCLASS()
class SURVIVEGAME_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();
	//MovementControl callback
	void MoveForward(const FInputActionValue& val);
	void MoveBackward(const FInputActionValue& val);
	void MoveLeft(const FInputActionValue& val);
	void MoveRight(const FInputActionValue& val);
	//InteractOption callback
	void Option1Selected();
	void Option2Selected();
	void Option3Selected();
	void Option4Selected();
	//InteractManagerComponent Status callback
	void SetSelectFromBackSocket();
	void SetSelectAsSourceInteractObject();
	//CameraControl callback
	void CameraYawRotate(const FInputActionValue& val);
	void CameraPitchRotate(const FInputActionValue& val);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//InputMapping
	UPROPERTY(EditAnywhere,Category="InputMapping")
	UInputMappingContext* CharacterInputMapping;
	//InteractManagerComponent
	UPROPERTY(EditAnywhere,Category="InteractManager")
	UInteractManagerComponent* InteractManagerComponent;
	UPROPERTY(EditAnywhere,Category="InteractManager")
	UBoxComponent* InteractObjectDetector;
	
	//CameraAction
	UPROPERTY(EditAnywhere,Category="Action")
	UInputAction* CameraYaw;
	UPROPERTY(EditAnywhere,Category="Action")
	UInputAction* CameraPitch;

	//MovementAction
	UPROPERTY(EditAnywhere,Category="Action")
	UInputAction* Forward;
	UPROPERTY(EditAnywhere,Category="Action")
	UInputAction* Backward;
	UPROPERTY(EditAnywhere,Category="Action")
	UInputAction* Left;
	UPROPERTY(EditAnywhere,Category="Action")
	UInputAction* Right;
	//Interact Action
	UPROPERTY(EditAnywhere,Category="Action")
	UInputAction* Focus;
	UPROPERTY(EditAnywhere,Category="Action")
	UInputAction* SwitchInRangeLeft;
	UPROPERTY(EditAnywhere,Category="Action")
	UInputAction* SwitchInRangeRight;
	UPROPERTY(EditAnywhere,Category="Action")
	UInputAction* SelectAsSourceObject;
	UPROPERTY(EditAnywhere,Category="Action")
	UInputAction* Option1;
	UPROPERTY(EditAnywhere,Category="Action")
	UInputAction* Option2;
	UPROPERTY(EditAnywhere,Category="Action")
	UInputAction* Option3;
	UPROPERTY(EditAnywhere,Category="Action")
	UInputAction* Option4;
	//InteractComponentStatusEnum
	UPROPERTY(EditAnywhere,Category="Action")
	UInputAction* SelectFromBackSocket;
	//EquipmentRelated
	UPROPERTY(EditAnywhere,Category="Action")
	UInputAction* EquipProjectileEquipment;
	UPROPERTY(EditAnywhere,Category="Action")
	UInputAction* EquipWearEquipment;
	UPROPERTY(EditAnywhere,Category="Action")
	UInputAction* EquipConstructEquipment;
	UPROPERTY(EditAnywhere,Category="Action")
	UInputAction* SwitchEquipmentPrevious;
	UPROPERTY(EditAnywhere,Category="Action")
	UInputAction* SwitchEquipmentNext;
	UPROPERTY(EditAnywhere,Category="Action")
	UInputAction* AimEquipment;
	UPROPERTY(EditAnywhere,Category="Action")
	UInputAction* UseEquipment;
	UPROPERTY(EditAnywhere,Category="Action")
	UInputAction* EquipmentSpecial1;
	UPROPERTY(EditAnywhere,Category="Action")
	UInputAction* EquipmentSpecial2;
	//CameraInput
	FVector2D CameraInput;

	//MovementInput
	FVector2D MovementInput;
	
	//WeightAffectedMovement
	UPROPERTY(EditAnywhere,Category="WeightAffectedMovement")
	FString WeightStatus;
	UPROPERTY(EditAnywhere,Category="WeightAffectedMovement")
	UDataTable* WeightAffectedMovementDataTable;
	UPROPERTY(EditAnywhere,Category="WeightAffectedMovement")
	float GroundFriction;
	UPROPERTY(EditAnywhere,Category="WeightAffectedMovement")
	float BrakingFrictionFactor;
	UPROPERTY(EditAnywhere,Category="WeightAffectedMovement")
	float MaxAcceleration;

	//GameState
	UPROPERTY()
	AMainGameState* MainGameState;
	UPROPERTY()  
	AMainPlayerState* MainPlayerState;	
public:
	//PhysicsConstrain
	UPROPERTY(EditAnywhere,Category="Sockets")
	UStaticMeshComponent* BackSocket;
	UPROPERTY(EditAnywhere,Category="Sockets")
	UStaticMeshComponent* RHandSocket;
	//CameraComponents
	UPROPERTY(EditAnywhere,Category="Camera")
	USpringArmComponent* CamSpringArm;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Camera")
	UCameraComponent* MainCamera;
	UPROPERTY(EditAnywhere,Category="Camera")
	USphereComponent* CameraTrigger;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void WeightChange();
};
