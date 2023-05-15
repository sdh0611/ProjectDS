// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class ProjectDS : ModuleRules
{
	public ProjectDS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] { "ProjectDSFramework" });

        PublicIncludePaths.AddRange(
            new string[]
            {
                        "ProjectDS/Public",
                        "ProjectDS/Public/Character",
                        "ProjectDS/Public/Character/Inventory",
                        "ProjectDS/Public/Character/Anim",
                        //"ProjectDS/Public/Game",
                        "ProjectDS/Public/Player",
                        "ProjectDSFramework/Public",
                        "ProjectDSFramework/Public/Character",
                        "ProjectDSFramework/Public/Character/Inventory",
                        "ProjectDSFramework/Public/Game",
                        "ProjectDSFramework/Public/Player",
            }
        );

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
