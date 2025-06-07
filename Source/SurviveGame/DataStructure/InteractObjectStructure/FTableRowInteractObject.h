// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FTableRowInteractObject.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FTableRowInteractObject : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	FTableRowInteractObject();
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,DisplayName = "Option1")
	FText Option1;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,DisplayName = "EventID1")
	int EventID1;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,DisplayName = "Option2")
	FText Option2;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,DisplayName = "EventID2")
	int EventID2;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,DisplayName = "Option3")
	FText Option3;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,DisplayName = "EventID3")
	int EventID3;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,DisplayName = "Option4")
	FText Option4;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,DisplayName = "EventID4")
	int EventID4;
	~FTableRowInteractObject();
};
