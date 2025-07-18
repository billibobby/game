#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MultiplayerGameMode.generated.h"

UCLASS()
class MULTIPLAYERGAME_API AMultiplayerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMultiplayerGameMode();

protected:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TSubclassOf<class APawn> DefaultPawnClass;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	int32 MaxPlayers = 4;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float RespawnDelay = 3.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TArray<class APlayerStart*> SpawnPoints;

public:
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void RespawnPlayer(AController* PlayerController);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void EndMatch();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void RestartMatch();
}; 