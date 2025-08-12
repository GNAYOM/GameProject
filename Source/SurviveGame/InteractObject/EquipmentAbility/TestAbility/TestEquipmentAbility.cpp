#include "TestEquipmentAbility.h"

UTestEquipmentAbility::UTestEquipmentAbility()
{
}

void UTestEquipmentAbility::BeginPlay()
{
	Super::BeginPlay();
}

void UTestEquipmentAbility::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTestEquipmentAbility::UseEquipment()
{
	Super::UseEquipment();
}
