// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceUIBase.h"

#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
USpaceUIBase::USpaceUIBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


FVector USpaceUIBase::MainCameraWorldLocation()
{
	TArray<AActor*> MainCharacterFound;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AInteractObject::StaticClass(),MainCharacterFound);
	for (AActor* FoundActor : MainCharacterFound )
	{
		MainCharacter = Cast<ATestCharacter>(FoundActor);
		if (MainCharacter)
		{
			break;
		}
	}
	MainCameraLocation = MainCharacter->FindComponentByClass<UCameraComponent>()->GetComponentLocation();
	return MainCameraLocation;
}

// Called when the game starts
void USpaceUIBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USpaceUIBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

