// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ContentBrowserDelegates.h"
#include "ComponentReregisterContext.h"
#include "Dialogs/DlgPickAssetPath.h"
#include "AssetRegistryModule.h"
#include "AssetToolsModule.h"
#include "AssetData.h"
#include "Interfaces/IMainFrameModule.h"
#include "EditorStyleSet.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "EditorDirectories.h"

#include "GeometryCache.h"
#include "GeometryCacheTrackStreamable.h"
#include "GeometryCacheMeshData.h"

/**
 * 
 */
class GALEMBICUTILITIES_API GAlembicCBExtension
{
public:
	GAlembicCBExtension();
	~GAlembicCBExtension();


	static TSharedRef<FExtender> OnExtendContentBrowserAssetMenu(const TArray<FAssetData>& SelectedAssets);
	static void InstallHooks();
	static void RemoveHooks();

	static TArray<FContentBrowserMenuExtender_SelectedAssets>& GetAssetExtenderDelegates();
	static void CreateAssetActionsSubMenu(FMenuBuilder& MenuBuilder, TArray<FAssetData> SelectedAssets);

	static void AssignMaterials(TArray<FAssetData> SelectedAssets);

};
