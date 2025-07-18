#include "MultiplayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"
#include "Projectile.h"

AMultiplayerCharacter::AMultiplayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Set our turn rates for input
	TurnRateGamepad = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Create weapon mesh
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(FollowCamera);

	// Initialize health
	CurrentHealth = MaxHealth;

	// Enable replication
	bReplicates = true;
}

void AMultiplayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMultiplayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMultiplayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMultiplayerCharacter::Fire);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AMultiplayerCharacter::Reload);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMultiplayerCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMultiplayerCharacter::StopSprint);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AMultiplayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AMultiplayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
}

void AMultiplayerCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMultiplayerCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AMultiplayerCharacter::StartSprint()
{
	if (!bIsSprinting)
	{
		bIsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void AMultiplayerCharacter::StopSprint()
{
	if (bIsSprinting)
	{
		bIsSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void AMultiplayerCharacter::Fire()
{
	if (!bCanFire || !IsAlive())
		return;

	// Server-side fire logic
	if (HasAuthority())
	{
		// Spawn projectile
		if (ProjectileClass)
		{
			FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * MuzzleOffset.X + GetActorUpVector() * MuzzleOffset.Z;
			FRotator SpawnRotation = GetActorRotation();

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = this;

			AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
			if (Projectile)
			{
				Projectile->SetOwner(this);
			}
		}

		// Set fire rate timer
		bCanFire = false;
		GetWorldTimerManager().SetTimer(FireTimerHandle, [this]() { bCanFire = true; }, FireRate, false);
	}
}

void AMultiplayerCharacter::Reload()
{
	// Reload logic can be implemented here
	UE_LOG(LogTemp, Warning, TEXT("Reloading..."));
}

void AMultiplayerCharacter::TakeDamage(float DamageAmount, AController* EventInstigator, AActor* DamageCauser)
{
	if (!HasAuthority())
		return;

	CurrentHealth = FMath::Max(0.0f, CurrentHealth - DamageAmount);

	if (CurrentHealth <= 0.0f)
	{
		// Player died
		AddDeath();
		
		// Notify game mode
		if (AMultiplayerGameMode* GameMode = Cast<AMultiplayerGameMode>(GetWorld()->GetAuthGameMode()))
		{
			GameMode->RespawnPlayer(GetController());
		}
	}
}

void AMultiplayerCharacter::Heal(float HealAmount)
{
	if (!HasAuthority())
		return;

	CurrentHealth = FMath::Min(MaxHealth, CurrentHealth + HealAmount);
}

void AMultiplayerCharacter::AddScore(int32 Points)
{
	if (!HasAuthority())
		return;

	Score += Points;
}

void AMultiplayerCharacter::AddKill()
{
	if (!HasAuthority())
		return;

	Kills++;
	AddScore(10);
}

void AMultiplayerCharacter::AddDeath()
{
	if (!HasAuthority())
		return;

	Deaths++;
}

void AMultiplayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMultiplayerCharacter, CurrentHealth);
	DOREPLIFETIME(AMultiplayerCharacter, Score);
	DOREPLIFETIME(AMultiplayerCharacter, Kills);
	DOREPLIFETIME(AMultiplayerCharacter, Deaths);
} 