// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FTableRowProperties.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FTableRowProperties : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	FTableRowProperties();
	UPROPERTY(EditAnywhere,BlueprintReadOnly,DisplayName = "Weight")
	float Weight;

	~FTableRowProperties();
};
