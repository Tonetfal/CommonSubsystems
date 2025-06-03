// Author: Antonio Sidenko (Tonetfal), July 2023

using UnrealBuildTool;

public class CommonSubsystems : ModuleRules
{
	public CommonSubsystems(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
			);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
			}
			);
	}
}
