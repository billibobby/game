#include "MultiplayerGameMode.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "MultiplayerCharacter.h"

AMultiplayerGameMode::AMultiplayerGameMode()
{
	// Set default pawn class to our character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_MultiplayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	else
	{
		DefaultPawnClass = AMultiplayerCharacter::StaticClass();
	}

	// Set default values
	MaxPlayers = 4;
	RespawnDelay = 3.0f;
}

void AMultiplayerGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Find all player start points
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundActors);
	
	for (AActor* Actor : FoundActors)
	{
		if (APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
		{
			SpawnPoints.Add(PlayerStart);
		}
	}
}

void AMultiplayerGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (NewPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player %s joined the game"), *NewPlayer->GetName());
	}
}

void AMultiplayerGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	if (Exiting)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player %s left the game"), *Exiting->GetName());
	}
}

void AMultiplayerGameMode::RespawnPlayer(AController* PlayerController)
{
	if (!PlayerController || !GetWorld())
		return;

	// Find a random spawn point
	APlayerStart* SpawnPoint = nullptr;
	if (SpawnPoints.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
		SpawnPoint = SpawnPoints[RandomIndex];
	}

	// Spawn the player
	if (SpawnPoint)
	{
		FVector SpawnLocation = SpawnPoint->GetActorLocation();
		FRotator SpawnRotation = SpawnPoint->GetActorRotation();

		APawn* NewPawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, SpawnLocation, SpawnRotation);
		if (NewPawn)
		{
			PlayerController->Possess(NewPawn);
		}
	}
}

void AMultiplayerGameMode::EndMatch()
{
	// End the current match
	UE_LOG(LogTemp, Warning, TEXT("Match ended"));
}

void AMultiplayerGameMode::RestartMatch()
{
	// Restart the match
	UE_LOG(LogTemp, Warning, TEXT("Match restarted"));
	
	// Respawn all players
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = Iterator->Get();
		if (PlayerController)
		{
			RespawnPlayer(PlayerController);
		}
	}
} 