// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PropertyHandle.h"
#include "IDetailCustomization.h"
#include "AssetSelectionOption.h"
#include "PropertyCustomizationHelpers.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"

/**
 * 
 */
class GALEMBICUTILITIES_API ASelectionDetailCustomization : public IDetailCustomization
{
public:

	static TSharedRef< IDetailCustomization > MakeInstance();
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

	bool OnFilterAsset(const FAssetData& FilteredAsset);
	void OnAssetSelectionChanged(const FAssetData& SelectedAsset, TSharedRef<IPropertyHandle> PropertyHandle);

	TArray<UObject*> TargetAssets;
};
