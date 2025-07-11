// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include "SurviveGame/DataStructure/BaseCharacterStructure/WeightAffectedMovement/FTableRowWeightAffectedMovement.h"
#include "SurviveGame/GameState/MainGameState.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;


	
	CamSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraComponent"));
	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	CameraTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("Camera Trigger"));
	CamSpringArm->SetupAttachment(RootComponent);
	CamSpringArm->TargetArmLength = 400.f;
	CamSpringArm->bEnableCameraLag = true;
	MainCamera->SetupAttachment(CamSpringArm,USpringArmComponent::SocketName);
	CameraTrigger->SetupAttachment(MainCamera);
	
	BackSocket = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackSocket"));
	BackSocket->SetupAttachment(GetMesh());

	CharacterInputMapping = CreateDefaultSubobject<UInputMappingContext>(TEXT("InputMapping"));
	InteractObjectDetector = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractObjectDetector"));
	InteractObjectDetector -> SetupAttachment(GetMesh());
	InteractManagerComponent = CreateDefaultSubobject<UInteractManagerComponent>(TEXT("InteractManager"));
	InteractManagerComponent -> SetupAttachment(RootComponent);

}
//MovementControl callback
void ABaseCharacter::MoveForward(const FInputActionValue& val)
{
	//UE_LOG(LogTemp, Warning, TEXT("TestCharacter::MoveForward"))
	MovementInput.X += 1.f;
}

void ABaseCharacter::MoveBackward(const FInputActionValue& val)
{
	//UE_LOG(LogTemp, Warning, TEXT("TestCharacter::MoveBackward"));
	MovementInput.X -= 1.f;
}

void ABaseCharacter::MoveLeft(const FInputActionValue& val)
{
	//UE_LOG(LogTemp, Warning, TEXT("TestCharacter::MoveLeft"));
	MovementInput.Y -= 1.f;
}

void ABaseCharacter::MoveRight(const FInputActionValue& val)
{
	//UE_LOG(LogTemp, Warning, TEXT("TestCharacter::MoveRight"));
	MovementInput.Y += 1.f;
}

//CameraControl callback
void ABaseCharacter::CameraYawRotate(const FInputActionValue& val)
{
	//UE_LOG(LogTemp,Warning, TEXT("YawInput%f"),val.Get<float>());
	CameraInput.X = val.Get<float>();
}

void ABaseCharacter::CameraPitchRotate(const FInputActionValue& val)
{
	//UE_LOG(LogTemp,Warning, TEXT("PitchInput%f"),val.Get<float>());
	CameraInput.Y = val.Get<float>();
}

//OptionSelected callback
void ABaseCharacter::Option1Selected()
{
	//UE_LOG(LogTemp,Warning,TEXT("Option1Selected"));
	InteractManagerComponent->InteractObjectInputDetected(InputEnum::Option1);
}

void ABaseCharacter::Option2Selected()
{
	//UE_LOG(LogTemp,Warning,TEXT("Option2Selected"));
	InteractManagerComponent->InteractObjectInputDetected(InputEnum::Option2);
}

void ABaseCharacter::Option3Selected()
{
	//UE_LOG(LogTemp,Warning,TEXT("Option3Selected"));
	InteractManagerComponent->InteractObjectInputDetected(InputEnum::Option3);
}

void ABaseCharacter::Option4Selected()
{
	//UE_LOG(LogTemp,Warning,TEXT("Option4Selected"));
	InteractManagerComponent->InteractObjectInputDetected(InputEnum::Option4);
}

void ABaseCharacter::SetSelectFromBackSocket()
{
	if(MainPlayerState->InteractManagerComponentStatus != InteractManagerComponentStatusEnum::SelectFromBackSocket)
		MainPlayerState->InteractManagerComponentStatus =  InteractManagerComponentStatusEnum::SelectFromBackSocket;
	else
	{
		MainPlayerState->InteractManagerComponentStatus = InteractManagerComponentStatusEnum::SelectByRange;
	}
}


// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	MainGameState = GetWorld()->GetAuthGameMode()->GetGameState<AMainGameState>();
	MainPlayerState = GetWorld()->GetFirstPlayerController()->GetPlayerState<AMainPlayerState>(); 
	MainPlayerState -> BackSocket = BackSocket;
	WeightAffectedMovementDataTable = LoadObject<UDataTable>(this
		,TEXT("/Script/Engine.DataTable'/Game/GameContent/DataTable/Character/BaseCharacter/WeightSystem/WeightAffectedMovementDataTable.WeightAffectedMovementDataTable'"));
	WeightChange();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//相机控制
	{
		FRotator CamDeltaRotator = CamSpringArm->GetComponentRotation();
		CamDeltaRotator.Pitch = FMath::Clamp(CamDeltaRotator.Pitch + CameraInput.Y,-80.0f,80.0f);
		CamDeltaRotator.Yaw += CameraInput.X;
		CamSpringArm->SetWorldRotation(CamDeltaRotator);
		CameraInput.Y = 0.f;
		CameraInput.X = 0.f;
	}//cam delta pitch rotator
	if(MainPlayerState->PossessedSystem->ConnectionMatrix.Num() != 0)
	MainGameState->PrintConnectionMatrix(*MainPlayerState->PossessedSystem);
	/*{
		FRotator CamDeltaRotator = CamSpringArm->GetComponentRotation();

		CamSpringArm->SetWorldRotation(CamDeltaRotator);
		
		
	}*///cam delta yaw rotator
	//设置操控角色新状态并将记录新增状态的变量归零
	{
		FRotator RawVelocityRotator = FRotator(0, CamSpringArm->GetComponentRotation().Yaw
			,0);
		FVector RawVelocityRotated = RawVelocityRotator.RotateVector(FVector(MovementInput.X
			, MovementInput.Y, 0.f).GetSafeNormal());
		AddMovementInput(RawVelocityRotated);
		if(GetVelocity().Length() <= 100 && MovementInput == FVector2D::Zero())
		{
			GetCharacterMovement()->Velocity = FVector::Zero();
		}//当速度小于5并且没有输入时将速度归零
		MovementInput.X = 0.f;
		MovementInput.Y = 0.f;
	}
	// 设置 skeleton mesh 朝向
	{
		FVector CurrentDirectionNormal = GetMesh()->GetForwardVector().GetSafeNormal();
		FVector CurrentVelocityNormal = GetVelocity().GetSafeNormal();
		if (!GetVelocity().IsNearlyZero(5))
		{
			CurrentDirectionNormal = FVector::SlerpVectorToDirection(CurrentDirectionNormal,CurrentVelocityNormal
				,FMath::Lerp(0,0.2,GetCharacterMovement()->Velocity.Length()/200.0f));
			
			//UE_LOG(LogTemp, Warning, TEXT("%f"), FMath::Lerp(0,0.2,GetCharacterMovement()->Velocity.Length()/200.0f))
			GetMesh()->SetWorldRotation(CurrentDirectionNormal.Rotation());
		}
		MainPlayerState->CurrentDirectionNormal = CurrentDirectionNormal;		
	}
	//更新Player状态
	{
		MainPlayerState->BackSocketCurrentLocation = BackSocket->GetComponentLocation();
	}
}


// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if(PlayerController != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem *InputSubsystem
		= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if(InputSubsystem != nullptr)
		{
			InputSubsystem -> AddMappingContext(CharacterInputMapping,100);
		}
	}
	
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(this->InputComponent);
	if(EnhancedInputComponent != nullptr)
	{
		if(Forward)
		{
			EnhancedInputComponent -> BindAction(Forward,ETriggerEvent::Triggered,this
				,&ABaseCharacter::MoveForward);
		}
		if(Backward)
		{
			EnhancedInputComponent -> BindAction(Backward,ETriggerEvent::Triggered,this
				,&ABaseCharacter::MoveBackward);
		}
		if(Right)
		{
			EnhancedInputComponent -> BindAction(Right,ETriggerEvent::Triggered,this
				,&ABaseCharacter::MoveRight);
		}
		if(Left)
		{
			EnhancedInputComponent -> BindAction(Left,ETriggerEvent::Triggered,this
				,&ABaseCharacter::MoveLeft);
		}
		if(CameraYaw)
		{
			EnhancedInputComponent -> BindAction(CameraYaw,ETriggerEvent::Triggered,this
				,&ABaseCharacter::CameraYawRotate);
		}
		if(CameraPitch)
		{
			EnhancedInputComponent -> BindAction(CameraPitch,ETriggerEvent::Triggered,this
				,&ABaseCharacter::CameraPitchRotate);
		}
		if (Option1)
		{
			EnhancedInputComponent -> BindAction(Option1,ETriggerEvent::Triggered,this
				,&ABaseCharacter::Option1Selected);
		}
		if (Option2)
		{
			EnhancedInputComponent -> BindAction(Option2,ETriggerEvent::Triggered,this
				,&ABaseCharacter::Option2Selected);
		}
		if (Option3)
		{
			EnhancedInputComponent -> BindAction(Option3,ETriggerEvent::Triggered,this
				,&ABaseCharacter::Option3Selected);
			
		}
		if (Option4)
		{
			EnhancedInputComponent -> BindAction(Option4,ETriggerEvent::Triggered,this
				,&ABaseCharacter::Option4Selected);
		}
		if(SelectFromBackSocket)
		{
			
			EnhancedInputComponent -> BindAction(SelectFromBackSocket,ETriggerEvent::Triggered,this
				,&ABaseCharacter::SetSelectFromBackSocket);
		}
	}
}

void ABaseCharacter::WeightChange()
{
	float g = MainPlayerState->WeightLoad;
	UE_LOG(LogTemp,Warning,TEXT("weight %f"),g);
	if (WeightAffectedMovementDataTable)
	{
		//for (FName RowName : WeightAffectedMovementDataTable->GetRowNames())
		//{
			//UE_LOG(LogTemp, Warning, TEXT("RowName: %s"), *RowName.ToString());
			FTableRowWeightAffectedMovement* WeightAffectedMovementData =
				WeightAffectedMovementDataTable->FindRow<FTableRowWeightAffectedMovement>(FName(WeightStatus),TEXT(""));
			if(WeightAffectedMovementData)
			{
				GroundFriction = WeightAffectedMovementData -> GroundFriction;
				BrakingFrictionFactor = WeightAffectedMovementData -> BrakingFrictionFactor;
				MaxAcceleration = WeightAffectedMovementData -> MaxAcceleration;
				GetCharacterMovement() -> GroundFriction = GroundFriction;
				GetCharacterMovement() -> BrakingFrictionFactor = BrakingFrictionFactor;
				GetCharacterMovement() -> MaxAcceleration = MaxAcceleration;
				
			}
		//}
	}
}
