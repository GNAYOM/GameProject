// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceUI.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASpaceUI::ASpaceUI()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


float ASpaceUI::RightAngleYawRotate(float YawRotation, float AbsAllowableRange)
{
	float tmp_ResultYawRotation = 0;
	if (0 <= abs(YawRotation) && abs(YawRotation) < 45)
	{
		tmp_ResultYawRotation = 0;
	}
	else if (45 <= abs(YawRotation) && abs(YawRotation) < 135)
	{
		if(YawRotation >0)
			tmp_ResultYawRotation = 90;
		else
		{
			tmp_ResultYawRotation = -90;
		}
	}
	else if (135 <= abs(YawRotation) && abs(YawRotation) < 180)
	{
		if(YawRotation > 0)
			tmp_ResultYawRotation = 179.999;
		else
		{
			tmp_ResultYawRotation = -179.999;
		}
	}
	tmp_ResultYawRotation -= FMath::Clamp(tmp_ResultYawRotation - YawRotation ,-AbsAllowableRange,AbsAllowableRange);
	return tmp_ResultYawRotation;
}

// Called when the game starts or when spawned
void ASpaceUI::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> MainCharacterFound;
	MainCharacter = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	/*for (AActor* FoundActor : MainCharacterFound )
	{
		MainCharacter = Cast<ATestCharacter>(FoundActor);
		UE_LOG(LogTemp,Warning,TEXT("FOUND actor"));
		UE_LOG(LogTemp,Warning,TEXT("%s"),*FoundActor->GetHumanReadableName());
		if (MainCharacter)
		{
			UE_LOG(LogTemp,Warning,TEXT("FOUND ATEST"));
			break;
		}
	}*/
	if(MainCharacter)
	{
		UE_LOG(LogTemp,Warning,TEXT("%s"),*MainCharacter->GetHumanReadableName());
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("NotFound"));
	}
}

// Called every frame
void ASpaceUI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

FVector ASpaceUI::MainCameraWorldLocation()
{
	MainCameraLocation = MainCharacter->FindComponentByClass<UCameraComponent>()->GetComponentLocation();
	return MainCameraLocation;
}