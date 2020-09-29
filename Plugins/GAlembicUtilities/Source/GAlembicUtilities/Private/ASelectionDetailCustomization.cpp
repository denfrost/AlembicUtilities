// Fill out your copyright notice in the Description page of Project Settings.


#include "ASelectionDetailCustomization.h"
#include "GeometryCache.h"


#define LOCTEXT_NAMESPACE "AssetSelectionCustomization"


TSharedRef< IDetailCustomization > ASelectionDetailCustomization::MakeInstance()
{
	return MakeShareable(new ASelectionDetailCustomization);
}

void ASelectionDetailCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{

	TSharedRef< IPropertyHandle > SourceAssetProperty = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UAssetSelectionOption, SourceAsset));
	SourceAssetProperty->ResetToDefault();
	DetailBuilder.HideProperty(SourceAssetProperty);

	TArray< TWeakObjectPtr< UObject > > Objects;
	DetailBuilder.GetObjectsBeingCustomized(Objects);
	if (Objects.Num() == 0)
	{
		return;
	}
	TWeakObjectPtr< UAssetSelectionOption > CurrentObject = Cast< UAssetSelectionOption >(Objects[0].Get());
	TargetAssets = CurrentObject->TargetAssets;

	FOnShouldFilterAsset AssetFilter = FOnShouldFilterAsset::CreateSP(this, &ASelectionDetailCustomization::OnFilterAsset);
	FOnSetObject SetObject = FOnSetObject::CreateSP(this, &ASelectionDetailCustomization::OnAssetSelectionChanged, SourceAssetProperty);
	FOnShouldSetAsset SetAsset = FOnShouldSetAsset::CreateSP(this, &ASelectionDetailCustomization::OnFilterAsset);

	TSharedRef<SHorizontalBox> AssetSelectionBox =
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth().Padding(4)
		[
			SNew(STextBlock)
			.ToolTipText(FText::FromString("Choose a Source Asset"))
		.Text(FText::FromString("Source Asset"))
		]
	+ SHorizontalBox::Slot()
		.Padding(4)
		[
			SNew(SObjectPropertyEntryBox).PropertyHandle(SourceAssetProperty).DisplayBrowse(true).EnableContentPicker(true)
			.AllowedClass(UObject::StaticClass()).AllowClear(true).OnShouldSetAsset(SetAsset)
			.OnShouldFilterAsset(AssetFilter).OnObjectChanged(SetObject)

		];

	IDetailCategoryBuilder& DetailCatgoryBuilder = DetailBuilder.EditCategory(TEXT("AssetSelection"));
	DetailCatgoryBuilder.AddCustomRow(LOCTEXT("AssetSelection", "AssetSelection"))
		[
			AssetSelectionBox
		];


}


bool ASelectionDetailCustomization::OnFilterAsset(const FAssetData& FilteredAsset)
{
	if (FilteredAsset.IsValid())
	{
		
		//return the datatable whose row structure matches the given row structure
		UObject* currentobject = Cast<UObject>(FilteredAsset.GetAsset());
		if (currentobject && currentobject->IsA(UStaticMesh::StaticClass()) || currentobject->IsA(UGeometryCache::StaticClass()))
		{

			return false;
			

		}
		
	}

	return true;

}



void ASelectionDetailCustomization::OnAssetSelectionChanged(const FAssetData& SelectedAsset, TSharedRef<IPropertyHandle> PropertyHandle)
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


#undef LOCTEXT_NAMESPACE
