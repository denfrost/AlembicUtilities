// Fill out your copyright notice in the Description page of Project Settings.


#include "GAlembicCBExtension.h"
#include "ContentBrowserModule.h"

#include "SSourceAssetSelectionWidget.h"

#define LOCTEXT_NAMESPACE "GAlembic"

GAlembicCBExtension::GAlembicCBExtension()
{
}

GAlembicCBExtension::~GAlembicCBExtension()
{
}

//FContentBrowserMenuExtender_SelectedPaths ContentBrowserPathExtenderDelegate;
//FDelegateHandle ContentBrowserPathExtenderDelegateHandle;

FContentBrowserMenuExtender_SelectedAssets ContentBrowserAssetExtenderDelegate;
FDelegateHandle ContentBrowserAssetExtenderDelegateHandle;


void GAlembicCBExtension::InstallHooks()
{
	//ContentBrowserPathExtenderDelegate = FContentBrowserMenuExtender_SelectedPaths::CreateStatic(&FFlipspacesCBExtensions::OnExtendContentBrowserPathMenu);
	//TArray<FContentBrowserMenuExtender_SelectedPaths>& CBPathMenuExtenderDelegates = FFlipspacesCBExtensions::GetPathExtenderDelegates();
	//CBPathMenuExtenderDelegates.Add(ContentBrowserPathExtenderDelegate);
	//ContentBrowserPathExtenderDelegateHandle = CBPathMenuExtenderDelegates.Last().GetHandle();

	ContentBrowserAssetExtenderDelegate = FContentBrowserMenuExtender_SelectedAssets::CreateStatic(&GAlembicCBExtension::OnExtendContentBrowserAssetMenu);
	TArray<FContentBrowserMenuExtender_SelectedAssets>& CBAssetMenuExtenderDelegates = GAlembicCBExtension::GetAssetExtenderDelegates();
	CBAssetMenuExtenderDelegates.Add(ContentBrowserAssetExtenderDelegate);
	ContentBrowserAssetExtenderDelegateHandle = CBAssetMenuExtenderDelegates.Last().GetHandle();
}

void GAlembicCBExtension::RemoveHooks()
{
	//TArray<FContentBrowserMenuExtender_SelectedPaths>& CBPathMenuExtenderDelegates = FFlipspacesCBExtensions::GetPathExtenderDelegates();
	//CBPathMenuExtenderDelegates.RemoveAll([](const FContentBrowserMenuExtender_SelectedPaths& Delegate)
	//{
	//	return Delegate.GetHandle() == ContentBrowserPathExtenderDelegateHandle;
	//});

	TArray<FContentBrowserMenuExtender_SelectedAssets>& CBAssetMenuExtenderDelegates = GAlembicCBExtension::GetAssetExtenderDelegates();
	CBAssetMenuExtenderDelegates.RemoveAll([](const FContentBrowserMenuExtender_SelectedAssets& Delegate)
	{
		return Delegate.GetHandle() == ContentBrowserAssetExtenderDelegateHandle;
	});
}


TSharedRef<FExtender> GAlembicCBExtension::OnExtendContentBrowserAssetMenu(const TArray<FAssetData>& SelectedAssets)
{
	TSharedRef<FExtender> MenuExtender(new FExtender());

	bool isInteractiveSelected = false;
	bool isObjectManagerSelected = false;
	for (FAssetData SelectedAsset : SelectedAssets)
	{
		if (SelectedAsset.GetAsset()->GetClass() == UGeometryCache::StaticClass())
		{
			isInteractiveSelected = true;
		}
		else if (SelectedAsset.GetAsset()->GetClass() == UStaticMesh::StaticClass())
		{
			isInteractiveSelected = true;
		}
	}

	//if the selected assets are of the right type, we display the corresponding menu.
	if (isInteractiveSelected)
	{
		MenuExtender->AddMenuExtension(
			"GetAssetActions", EExtensionHook::After,
			TSharedPtr<FUICommandList>(),
			FMenuExtensionDelegate::CreateStatic(&GAlembicCBExtension::CreateAssetActionsSubMenu, SelectedAssets));
	}
	
	return MenuExtender;
}


void GAlembicCBExtension::CreateAssetActionsSubMenu(FMenuBuilder& MenuBuilder, TArray<FAssetData> SelectedAssets)
{
	//here we are adding our menu item to the menu builder
	MenuBuilder.BeginSection("ContentBrowserAlembicAsset", LOCTEXT("AlembicMenuHeading", "Alembic"));
	{
		MenuBuilder.AddMenuEntry(
			LOCTEXT("AssignMaterials", "Assign Materials"),
			LOCTEXT("AssignMaterialsTooltip", "Assign Materials to Slots from Reference"),
			FSlateIcon(),
			FUIAction(
				FExecuteAction::CreateStatic(&GAlembicCBExtension::AssignMaterials, SelectedAssets)
			)
		);

	}
	MenuBuilder.EndSection();
}


void GAlembicCBExtension::AssignMaterials(TArray<FAssetData> SelectedAssets)
{
	FString AssetPath;
	const FString DefaultFilesystemDirectory = FEditorDirectories::Get().GetLastDirectory(ELastDirectory::NEW_ASSET);
	if (DefaultFilesystemDirectory.IsEmpty() || !FPackageName::TryConvertFilenameToLongPackageName(DefaultFilesystemDirectory, AssetPath))
	{
		// No saved path, just use the game content root
		AssetPath = TEXT("/Game");
	}

	FOpenAssetDialogConfig OpenAssetDialogConfig;
	OpenAssetDialogConfig.DialogTitleOverride = LOCTEXT("OepnAssetDialogTitle", "Select Source File");
	OpenAssetDialogConfig.bAllowMultipleSelection = false;
	OpenAssetDialogConfig.DefaultPath = AssetPath;
	OpenAssetDialogConfig.AssetClassNames.Add(UStaticMesh::StaticClass()->GetFName());
	OpenAssetDialogConfig.AssetClassNames.Add(UGeometryCache::StaticClass()->GetFName());

	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	TArray<FAssetData> SourceAssets = ContentBrowserModule.Get().CreateModalOpenAssetDialog(OpenAssetDialogConfig);

	if (SourceAssets.Num() > 0)
	{
		UObject* CurrentSourceAsset = SourceAssets[0].GetAsset();
		TArray<UMaterialInterface*> SourceMaterials;
		if (UGeometryCache* SourceGeomCache = Cast<UGeometryCache>(CurrentSourceAsset))
		{
			SourceMaterials = SourceGeomCache->Materials;

		}

		else if (UStaticMesh* SourceMesh = Cast<UStaticMesh>(CurrentSourceAsset))
		{
			TArray<FStaticMaterial> staticmaterials = SourceMesh->StaticMaterials;
			SourceMaterials.AddUninitialized(staticmaterials.Num());
			for (int mIndex = 0; mIndex < staticmaterials.Num(); mIndex++)
			{
				UMaterialInterface* CurrentMaterial = staticmaterials[mIndex].MaterialInterface;
				if (CurrentMaterial)
				{
					SourceMaterials[mIndex] = CurrentMaterial;
				}
			}

		}

		for (int mIndex = 0; mIndex < SourceMaterials.Num(); mIndex++)
		{
			if (SourceMaterials[mIndex])
			{
				for (auto sAsset : SelectedAssets)
				{

					if (sAsset != SourceAssets[0])
					{
						UObject* CurrentTargetAsset = sAsset.GetAsset();

						if (UGeometryCache* TargetGeomCache = Cast<UGeometryCache>(CurrentTargetAsset))
						{
							
							if (TargetGeomCache->Materials.Num() > mIndex)
							{
								//ensure(PropertyHandle->SetValue(SelectedObject) == FPropertyAccess::Success);

								FlushRenderingCommands();

								TargetGeomCache->Materials[mIndex] = SourceMaterials[mIndex];
								TargetGeomCache->Modify();


							}
						}
						else if (UStaticMesh* TargetMesh = Cast<UStaticMesh>(CurrentTargetAsset))
						{

							if (TargetMesh->StaticMaterials.Num() > mIndex)
							{
								FlushRenderingCommands();

								TargetMesh->StaticMaterials[mIndex].MaterialInterface = SourceMaterials[mIndex];
								TargetMesh->Modify();

							}

						}
					}
				}
			}
		}

	}

	//TSharedRef<SWindow> AssignMaterialsWindow = SNew(SWindow)
	//	.Title(LOCTEXT("AssignMaterialsOption", "Select Source File"))
	//	.ClientSize(FVector2D(400, 700))
	//	.SupportsMinimize(false).SupportsMaximize(false);

	//TArray<UObject*> TargetAssets;

	//for (FAssetData SelectedAsset : SelectedAssets)
	//{
	//	//Check if the selected asset if of UFlipspacesObjectData class or of Object manager blueprint class, otherwise we won't display our menu.
	//	//we can use this validation logic to switch between different types of menus based on the type of asset selected
	//	if (UObject* tasset = SelectedAsset.GetAsset())
	//	{
	//		TargetAssets.Add(tasset);
	//	}
	//	
	//}
	////add the custom slate widget in the created window, where we have options that we want to initialize our Object manager BP with.
	//AssignMaterialsWindow->SetContent
	//(
	//	SNew(SSourceAssetSelectionWidget).WidgetWindow(AssignMaterialsWindow).TargetAssets(TargetAssets)
	//);

	////Get Parent window and add this newly created window within the parent window
	//TSharedPtr<SWindow> ParentWindow;
	//if (FModuleManager::Get().IsModuleLoaded("MainFrame"))
	//{
	//	IMainFrameModule& MainFrame = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
	//	ParentWindow = MainFrame.GetParentWindow();

	//}
	//FSlateApplication::Get().AddModalWindow(AssignMaterialsWindow, ParentWindow, false);

	

}

TArray<FContentBrowserMenuExtender_SelectedAssets>& GAlembicCBExtension::GetAssetExtenderDelegates()
{
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
	return ContentBrowserModule.GetAllAssetViewContextMenuExtenders();
}


#undef LOCTEXT_NAMESPACE
