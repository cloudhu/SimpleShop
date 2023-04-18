// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CloudGameplayMessageNodes : ModuleRules
{
	public CloudGameplayMessageNodes(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateDependencyModuleNames.AddRange(
		new string[]
			{
				"BlueprintGraph",
				"Core",
				"CoreUObject",
				"Engine",
				"KismetCompiler",
				"PropertyEditor",
				"SimpleShop",
				"UnrealEd"
			}
        );

		PrivateIncludePaths.AddRange(
			new string[]
			{
			}
		);

		PublicIncludePaths.AddRange(
			new string[]
			{
			}
		);
	}
}
