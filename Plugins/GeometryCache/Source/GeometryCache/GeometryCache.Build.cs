// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class GeometryCache : ModuleRules
{
	public GeometryCache(ReadOnlyTargetRules Target) : base(Target)
	{

		var EngineDir = Path.GetFullPath(Target.RelativeEnginePath);

		var ShaderDirectory = Path.Combine(EngineDir, "Shaders", "Shared");
		PrivateIncludePaths.Add(ShaderDirectory);

        PublicDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "InputCore",
                "RenderCore",
                "RHI"
			}
		);

        PublicIncludePathModuleNames.Add("TargetPlatform");

        if (Target.bBuildEditor)
        {
            PublicIncludePathModuleNames.Add("GeometryCacheEd");
            DynamicallyLoadedModuleNames.Add("GeometryCacheEd");
            PrivateDependencyModuleNames.Add("MeshUtilitiesCommon");
        }        
	}
}
