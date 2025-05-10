// Fill out your copyright notice in the Description page of Project Settings.


#include "TestCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include "SurviveGame/DataStructure/TestCharacterStructure/WeightAffectedMovement/FTableRowWeightAffectedMovement.h"

// Sets default values
ATestCharacter::ATestCharacter()
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
	CharacterInputMapping = CreateDefaultSubobject<UInputMappingContext>(TEXT("InputMapping"));
	InteractObjectDetector = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractObjectDetector"));
	InteractObjectDetector -> SetupAttachment(GetMesh());
	InteractManagerComponent = CreateDefaultSubobject<UInteractManagerComponent>(TEXT("InteractManager"));
	InteractManagerComponent -> SetupAttachment(RootComponent);
}
//MovementControl
void ATestCharacter::MoveForward(const FInputActionValue& val)
{
	//UE_LOG(LogTemp, Warning, TEXT("TestCharacter::MoveForward"))
	MovementInput.X += 1.f;
}

void ATestCharacter::MoveBackward(const FInputActionValue& val)
{
	//UE_LOG(LogTemp, Warning, TEXT("TestCharacter::MoveBackward"));
	MovementInput.X -= 1.f;
}

void ATestCharacter::MoveLeft(const FInputActionValue& val)
{
	//UE_LOG(LogTemp, Warning, TEXT("TestCharacter::MoveLeft"));
	MovementInput.Y -= 1.f;
}

void ATestCharacter::MoveRight(const FInputActionValue& val)
{
	//UE_LOG(LogTemp, Warning, TEXT("TestCharacter::MoveRight"));
	MovementInput.Y += 1.f;
}

// Called when the game starts or when spawned
void ATestCharacter::BeginPlay()
{
	Super::BeginPlay();
	WeightAffectedMovementDataTable = LoadObject<UDataTable>(this
		,TEXT("/Script/Engine.DataTable'/Game/GameContent/DataTable/TestCharacter/WeightSystem/WeightAffectedMovementDataTable.WeightAffectedMovementDataTable'"));
	WeightChange();
}
//CameraControl
void ATestCharacter::CameraYawRotate(const FInputActionValue& val)
{
	//UE_LOG(LogTemp,Warning, TEXT("YawInput%f"),val.Get<float>());
	CameraInput.X = val.Get<float>();
}

void ATestCharacter::CameraPitchRotate(const FInputActionValue& val)
{
	//UE_LOG(LogTemp,Warning, TEXT("PitchInput%f"),val.Get<float>());
	CameraInput.Y = val.Get<float>();
}
// Called every frame
void ATestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//相机控制
	{
		FRotator CamDeltaRotator = CamSpringArm->GetComponentRotation();
		CamDeltaRotator.Pitch = FMath::Clamp(CamDeltaRotator.Pitch + CameraInput.Y,-80.0f,80.0f);
		CamDeltaRotator.Yaw += CameraInput.X;
		CamSpringArm->SetWorldRotation(CamDeltaRotator);
		float X = CameraInput.X;
		float Y = CameraInput.Y;
		UE_LOG(LogTemp,Warning,TEXT("InputX%f InputY%f"),X,Y);
		CameraInput.Y = 0.f;
		CameraInput.X = 0.f;
	}//cam delta pitch rotator
	
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
	}
}


// Called to bind functionality to input
void ATestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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
				,&ATestCharacter::MoveForward);
		}
		if(Backward)
		{
			EnhancedInputComponent -> BindAction(Backward,ETriggerEvent::Triggered,this
				,&ATestCharacter::MoveBackward);
		}
		if(Right)
		{
			EnhancedInputComponent -> BindAction(Right,ETriggerEvent::Triggered,this
				,&ATestCharacter::MoveRight);
		}
		if(Left)
		{
			EnhancedInputComponent -> BindAction(Left,ETriggerEvent::Triggered,this
				,&ATestCharacter::MoveLeft);
		}
		if(CameraYaw)
		{
			EnhancedInputComponent -> BindAction(CameraYaw,ETriggerEvent::Triggered,this
				,&ATestCharacter::CameraYawRotate);
		}
		if(CameraPitch)
		{
			EnhancedInputComponent -> BindAction(CameraPitch,ETriggerEvent::Triggered,this
				,&ATestCharacter::CameraPitchRotate);
		}
	}
}

void ATestCharacter::WeightChange()
{
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
