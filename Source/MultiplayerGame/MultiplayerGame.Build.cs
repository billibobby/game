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
			"OnlineSubsystemUtils",
			"Networking",
			"NetCore",
			"CommonUI",
			"CommonInput",
			"ModularGameplay",
			"ModularGameplayActors",
			"GameplayMessageRuntime",
			"GameplayStateTreeModule",
			"MassEntity",
			"MassCommon",
			"MassMovement",
			"MassSpawner",
			"MassAIBehavior",
			"MassGameplayDebug",
			"GameplayInsights",
			"GameplayDebugger",
			"DeveloperSettings",
			"EngineSettings"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { 
			"Slate",
			"SlateCore",
			"UMG",
			"ApplicationCore",
			"RenderCore",
			"RHI"
		});
	}
} 