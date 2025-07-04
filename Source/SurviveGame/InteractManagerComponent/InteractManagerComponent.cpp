// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractManagerComponent.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UInteractManagerComponent::UInteractManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	RootSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("RootSkeletalMesh");
	// ...
}


// Called when the game starts
void UInteractManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	RootSkeletalMesh = GetAttachmentRootActor()->GetComponentByClass<USkeletalMeshComponent>();
	InteractObjectDetector = GetAttachmentRootActor()->GetComponentByClass<UBoxComponent>();
	if(InteractObjectDetector)
	{
		UE_LOG(LogTemp,Warning,TEXT("Complete"));
		InteractObjectDetector->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		InteractObjectDetector->OnComponentBeginOverlap.AddDynamic(this,&UInteractManagerComponent::OnInteractObjectBeginOverlap);
		InteractObjectDetector->OnComponentEndOverlap.AddDynamic(this,&UInteractManagerComponent::OnInteractObjectEndOverlapEnd);
	}
	
	// ...
	
}


// Called every frame
void UInteractManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	tmp_InteractObjectsSorted = InRangeInteractObjects;
	float tmp_CurrentDistanceA = 0.f;
	float tmp_CurrentDistanceB = 0.f;
	AActorInteractObjectInterface* tmp_PosSwap = NULL;
	//In range InteractObject Sort by distance
	for(int i = 0; i < tmp_InteractObjectsSorted.Num(); i++)
	{

		for(int j = i;j<tmp_InteractObjectsSorted.Num();j++)
		{
			tmp_CurrentDistanceA = (RootSkeletalMesh -> GetComponentLocation()
				- tmp_InteractObjectsSorted[i] -> GetComponentByClass<UBoxComponent>() -> GetComponentLocation()).Length();
			tmp_CurrentDistanceB = (RootSkeletalMesh -> GetComponentLocation()
				- tmp_InteractObjectsSorted[j] -> GetComponentByClass<UBoxComponent>() -> GetComponentLocation()).Length();
			if(tmp_CurrentDistanceA > tmp_CurrentDistanceB)
			{
				tmp_PosSwap = tmp_InteractObjectsSorted[j];
				tmp_InteractObjectsSorted[j] = tmp_InteractObjectsSorted[i];
				tmp_InteractObjectsSorted[i] = tmp_PosSwap;
			}
		}
	}
	for(int i =0;AActorInteractObjectInterface* IO : tmp_InteractObjectsSorted)
	{
		
		FString IOName = IO -> BehaviorStatus;
		//UE_LOG(LogTemp,Warning,TEXT("%d  %s"),i,*IOName);
		i++;
	}
	InteractObjectsSorted = tmp_InteractObjectsSorted;
	// ...
}

void UInteractManagerComponent::InteractObjectInputDetected(int Input)
{
	
	if(!InteractObjectsSorted.IsEmpty())
	{
		InteractObjectsSorted[0]->ReleaseEventActively(Input);
		//GetAttachmentRootActor()->GetComponentByClass<UPhysicsConstraintComponent>()->ConstraintActor2 = InteractObjectsSorted[0];
		
	}
}

void UInteractManagerComponent::OnInteractObjectBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	FString IOName = OtherActor->GetName();
	UE_LOG(LogTemp, Warning, TEXT("BeginOverlap:%s"), *IOName);
	AActorInteractObjectInterface* InsertNewInteractObject =  Cast<AActorInteractObject>(OtherActor);
	InRangeInteractObjects.Add(InsertNewInteractObject);
	//InsertNewInteractObject->ReleaseEventActive(1);
}

void UInteractManagerComponent::OnInteractObjectEndOverlapEnd(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	FString IOName = OtherActor->GetName();
	UE_LOG(LogTemp,Warning,TEXT("EndOverlap111:%s"),*IOName);
	AActorInteractObjectInterface* InsertNewInteractObject =  Cast<AActorInteractObject>(OtherActor);
	InRangeInteractObjects.Remove(InsertNewInteractObject);
}

