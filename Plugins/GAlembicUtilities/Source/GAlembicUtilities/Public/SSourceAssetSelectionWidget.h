// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "AssetSelectionOption.h"
#include "IDetailsView.h"
#include "PropertyCustomizationHelpers.h"

/**
 * 
 */
class GALEMBICUTILITIES_API SSourceAssetSelectionWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSourceAssetSelectionWidget)
	{}
	SLATE_ARGUMENT(TSharedPtr<SWindow>, WidgetWindow)
	SLATE_ARGUMENT(TArray<UObject*>, TargetAssets)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	TWeakPtr< SWindow > WidgetWindow;
	TSharedPtr<IDetailsView> DetailsPanel;
	TArray<UObject*> TargetAssets;

	UAssetSelectionOption* AssetSelectionOption;

	FReply OnAssign();
	FReply OnCancel();
	bool CanAssign();

	void OnCollectionMenuSelectionChanged(const FAssetData& SelectedAsset, TSharedRef<IPropertyHandle> PropertyHandle);


};
