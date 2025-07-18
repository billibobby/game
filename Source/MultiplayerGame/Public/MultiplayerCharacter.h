#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "MultiplayerCharacter.generated.h"

UCLASS()
class MULTIPLAYERGAME_API AMultiplayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMultiplayerCharacter();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float TurnRateGamepad = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float FireRate = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FVector MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.0f;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Health")
	float CurrentHealth;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Score")
	int32 Score = 0;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Score")
	int32 Kills = 0;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Score")
	int32 Deaths = 0;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveForward(float Value);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveRight(float Value);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void StartSprint();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void StopSprint();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Reload();

	UFUNCTION(BlueprintCallable, Category = "Health")
	void TakeDamage(float DamageAmount, AController* EventInstigator, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Heal(float HealAmount);

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Points);

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddKill();

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddDeath();

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealthPercent() const { return CurrentHealth / MaxHealth; }

	UFUNCTION(BlueprintPure, Category = "Health")
	bool IsAlive() const { return CurrentHealth > 0.0f; }

protected:
	UPROPERTY()
	FTimerHandle FireTimerHandle;

	UPROPERTY()
	bool bIsSprinting = false;

	UPROPERTY()
	bool bCanFire = true;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
}; 