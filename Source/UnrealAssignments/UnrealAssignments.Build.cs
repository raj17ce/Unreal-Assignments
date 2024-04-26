// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UnrealAssignments : ModuleRules
{
	public UnrealAssignments(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "HeadMountedDisplay", "NavigationSystem", "AIModule", "Niagara", "Assignment2" });

        PublicIncludePaths.AddRange(new string[] {
            "C:/Program Files/UE_5.2/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public",
            "D:/Work/Unreal Training/UnrealAssignments/Source/Assignment2/Public"
        });

        PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
