// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FTableRowScript.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FTableRowScript : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	FTableRowScript();
	UPROPERTY(EditAnywhere,BlueprintReadOnly,DisplayName = "ScriptPath")
	FString ScriptPath;
	~FTableRowScript();
};
