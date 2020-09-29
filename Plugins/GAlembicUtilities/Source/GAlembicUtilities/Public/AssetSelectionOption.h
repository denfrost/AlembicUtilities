// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AssetSelectionOption.generated.h"

/**
 * 
 */
UCLASS()
class GALEMBICUTILITIES_API UAssetSelectionOption : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "AssetSelection")
		UObject* SourceAsset;

	UPROPERTY()
		TArray<UObject*> TargetAssets;

};
