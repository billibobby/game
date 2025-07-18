using UnrealBuildTool;

public class MultiplayerGame : ModuleRules
{
	public MultiplayerGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"HeadMountedDisplay",
			"EnhancedInput",
			"GameplayTasks",
			"NavigationSystem",
			"AIModule",
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks",
			"OnlineSubsystem",
			"OnlineSubsystemUtils"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
} 