// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAlembicUtilities.h"
#include "GAlembicCBExtension.h"
#include "ASelectionDetailCustomization.h"

#define LOCTEXT_NAMESPACE "FGAlembicUtilitiesModule"

void FGAlembicUtilitiesModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	auto& PropertyModule = FModuleManager::LoadModuleChecked< FPropertyEditorModule >("PropertyEditor");

	PropertyModule.RegisterCustomClassLayout(
		"AssetSelectionOption",
		FOnGetDetailCustomizationInstance::CreateStatic(&ASelectionDetailCustomization::MakeInstance)
	);


	GAlembicCBExtension::InstallHooks();

}


void FGAlembicUtilitiesModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGAlembicUtilitiesModule, GAlembicUtilities)