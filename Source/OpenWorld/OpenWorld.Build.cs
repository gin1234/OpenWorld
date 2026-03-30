// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OpenWorld : ModuleRules
{
	public OpenWorld(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Puerts for TypeScript scripting
		PrivateDependencyModuleNames.Add("Puerts");

		PublicIncludePaths.AddRange(new string[] {
			"OpenWorld",
            "OpenWorld/TSEngine",
            "OpenWorld/TSEngine/core",
            "OpenWorld/TSEngine/core/config",
            "OpenWorld/TSEngine/core/entity",
            "OpenWorld/TSEngine/core/transport",
            "OpenWorld/TSEngine/wrap",
            "OpenWorld/TSEngine/common"
        });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
