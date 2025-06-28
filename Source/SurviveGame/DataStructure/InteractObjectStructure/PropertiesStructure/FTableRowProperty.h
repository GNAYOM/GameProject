// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FTableRowProperty.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FTableRowProperty : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	FTableRowProperty();
	UPROPERTY(EditAnywhere,BlueprintReadOnly,DisplayName = "Weight")
	float Weight;

	~FTableRowProperty();
};
