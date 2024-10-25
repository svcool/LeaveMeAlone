// LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LMAWeaponComponent.generated.h"

class ALMABaseWeapon;
class UAnimMontage;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LEAVEMEALONE_API ULMAWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULMAWeaponComponent();
	void Fire();
	UFUNCTION(BlueprintCallable) 
	void NoFire();
	void Reload();
	UFUNCTION(BlueprintCallable)
	bool GetCurrentWeaponAmmo(FAmmoWeapon& AmmoWeapon) const;
	UFUNCTION(BlueprintCallable)
	ALMABaseWeapon* GetWeaponObject() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<ALMABaseWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UAnimMontage* ReloadMontage;

	


	virtual void BeginPlay() override;

public:
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	UPROPERTY()
	ALMABaseWeapon* Weapon = nullptr;
	bool AnimReloading = false;//
	void SpawnWeapon();
	void InitAnimNotify(); //ф-я перезарядки
	void OnNotifyReloadFinished(USkeletalMeshComponent* SkeletalMesh); //колбэк на собыитя от делегата 
	bool CanReload() const; // условия корректной перезарядки

	UFUNCTION()
	void ClipEmmpty();
};
