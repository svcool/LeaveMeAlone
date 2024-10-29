
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LMABaseWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FClipEmpty);

class USkeletalMeshComponent;
class UNiagaraSystem;
class USoundWave;
//*********************************************************************************
USTRUCT(BlueprintType)
struct FAmmoWeapon
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 Bullets = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 Clips = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float FireRate = 0.0f; // частота стрельбы

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	bool Infinite = true;
};

//*********************************************************************************
UCLASS()
class LEAVEMEALONE_API ALMABaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALMABaseWeapon();
	void Fire();
	void NoFire();
	void ChangeClip();
bool CanReload() const; // условия корректной перезарядки
FAmmoWeapon GetCurrentAmmoWeapon() const { return CurrentAmmoWeapon; }

FClipEmpty ClipEmpty; //делегат
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	USkeletalMeshComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float TraceDistance = 800.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FAmmoWeapon AmmoWeapon{30, 0, 0.3f, true};

	FTimerHandle FireTimerHandle;
	
	void Shoot();

	virtual void BeginPlay() override;

	void DecrementBullets();
	bool IsCurrentClipEmpty() const;
	bool IsClipFull() const;

	//************Звук************//

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	USoundWave* ShootWave;
	//************Звук************//

	//************Спецэффекты************//
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	UNiagaraSystem* TraceEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FString TraceName = "Tracer";
	void SpawnTrace(const FVector& TraceStart, const FVector& TraceEnd);
	//************Спецэффекты************//
public:
	
	virtual void Tick(float DeltaTime) override;

private:
	FAmmoWeapon CurrentAmmoWeapon;
	
};
