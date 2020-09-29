// Fill out your copyright notice in the Description page of Project Settings.


#include "SSourceAssetSelectionWidget.h"
#include "SlateOptMacros.h"

#define LOCTEXT_NAMESPACE "AssetSelectionWidget"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSourceAssetSelectionWidget::Construct(const FArguments& InArgs)
{
	WidgetWindow = InArgs._WidgetWindow;
	TargetAssets = InArgs._TargetAssets;

	AssetSelectionOption = NewObject<UAssetSelectionOption>(GetTransientPackage(), TEXT("FObjectUISettings"), RF_Transactional);
	AssetSelectionOption->TargetAssets = TargetAssets;

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs(false, false, false, FDetailsViewArgs::HideNameArea);

	//detailpanel creates a detail view with the properties present in the given object. This let us easily create multiple options at once without having to create slate control seperately 
	//for every property. 
	DetailsPanel = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsPanel->SetObject(AssetSelectionOption);

	//FOnShouldFilterAsset AssetFilter = FOnShouldFilterAsset::CreateSP(this, &SSourceAssetSelectionWidget::OnFilterAsset);
	//FOnSetObject SetObject = FOnSetObject::CreateSP(this, &SSourceAssetSelectionWidget::OnCollectionMenuSelectionChanged, CollectionTableProperty);
	//FOnShouldSetAsset SetAsset = FOnShouldSetAsset::CreateSP(this, &SSourceAssetSelectionWidget::OnFilterAsset, SelectedRowScruct, false);


	//TSharedRef< IPropertyHandle > CollectionTableProperty = PropertyEditorModule.GetProperty(GET_MEMBER_NAME_CHECKED(UItemEditorObject, CollectionTable));
	//CollectionTableProperty->ResetToDefault();
	//DetailBuilder.HideProperty(CollectionTableProperty);

	TSharedPtr<SBox> InspectorBox;

	ChildSlot
		[
			SNew(SBorder)
			.BorderImage(FEditorStyle::GetBrush("Menu.Background"))
		.Padding(10)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SAssignNew(InspectorBox, SBox)
			.MaxDesiredHeight(650.0f)
		.WidthOverride(400.0f)
		[
			DetailsPanel.ToSharedRef()
		]
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Right)
		.Padding(2)
		[
			SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		[
			SNew(SButton)
			.HAlign(HAlign_Center)
		.Text(LOCTEXT("Assign", "Assign"))
		.ToolTipText(LOCTEXT("Assign_ToolTip", "Assign the materials from the Selected Source")).IsEnabled(&SSourceAssetSelectionWidget::CanAssign)
		.OnClicked(this, &SSourceAssetSelectionWidget::OnAssign)
		]
	+ SHorizontalBox::Slot()
		[
			SNew(SButton)
			.HAlign(HAlign_Center)
		.Text(LOCTEXT("Cancel", "Cancel"))
		.ToolTipText(LOCTEXT("Cancel_ToolTip", "Cancels assigning the materials"))
		.OnClicked(this, &SSourceAssetSelectionWidget::OnCancel)
		]
		]
		]
		];
}

bool SSourceAssetSelectionWidget::CanAssign()
{
	if (AssetSelectionOption)
	{
		if (AssetSelectionOption->SourceAsset)
		{
			return true;
		}
	}
	return false;
}

FReply SSourceAssetSelectionWidget::OnAssign()
{
	

	TSharedPtr<SWindow> OwnerWindow = WidgetWindow.Pin();

	if (OwnerWindow.IsValid())
	{
		OwnerWindow.Get()->RequestDestroyWindow();
	}

	return FReply::Handled();
}

FReply SSourceAssetSelectionWidget::OnCancel()
{
	TSharedPtr<SWindow> OwnerWindow = WidgetWindow.Pin();

	if (OwnerWindow.IsValid())
	{
		OwnerWindow.Get()->RequestDestroyWindow();
	}

	return FReply::Handled();
}

void SSourceAssetSelectionWidget::OnCollectionMenuSelectionChanged(const FAssetData& SelectedAsset, TSharedRef<IPropertyHandle> PropertyHandle)
{
	//get the selected datatable from the asset
	const UObject* SelectedObject = Cast<UObject>(SelectedAsset.GetAsset());
	if (SelectedObject)
	{
		//change the corresponding property in our UObject
		ensure(PropertyHandle->SetValue(SelectedObject) == FPropertyAccess::Success);
	}
	else
	{
		PropertyHandle->ResetToDefault();
	}
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE
