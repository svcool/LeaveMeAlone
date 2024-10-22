#include "Components/LMAWeaponComponent.h"
#include "Animations/LMAReloadFinishedAnimNotify.h"
#include "GameFramework/Character.h"
#include "Weapon/LMABaseWeapon.h"


//**************************************************************************************************************
ULMAWeaponComponent::ULMAWeaponComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

}
//**************************************************************************************************************
void ULMAWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapon();
	InitAnimNotify();
}
//**************************************************************************************************************
// void ULMAWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//} 
//**************************************************************************************************************
void ULMAWeaponComponent::SpawnWeapon()
{
	Weapon = GetWorld()->SpawnActor<ALMABaseWeapon>(WeaponClass);
	if (Weapon)
	{
		const auto Character = Cast<ACharacter>(GetOwner());
		if (Character)
		{
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
			Weapon->AttachToComponent(Character->GetMesh(), AttachmentRules, "r_Weapon_Socket");
			Weapon->ClipEmpty.AddDynamic(this, &ULMAWeaponComponent::ClipEmmpty); // подписка на делегат
		}
	}
}
//*****************************************************************************************************
void ULMAWeaponComponent::Fire() 
{

	if (Weapon && !AnimReloading)
	{
		Weapon->Fire();
	}
}
//*****************************************************************************************************
void ULMAWeaponComponent::NoFire()
{
	if (Weapon)
	{
		Weapon->NoFire();
	}
}
//*****************************************************************************************************
void ULMAWeaponComponent::Reload()
{
	ClipEmmpty();
}
//*****************************************************************************************************
void ULMAWeaponComponent::InitAnimNotify()
{
	if (!ReloadMontage)
		return;

	const auto NotifiesEvents = ReloadMontage->Notifies;
	for (auto NotifyEvent : NotifiesEvents)
	{
		auto ReloadFinish = Cast<ULMAReloadFinishedAnimNotify>(NotifyEvent.Notify);
		if (ReloadFinish)
		{
			ReloadFinish->OnNotifyReloadFinished.AddUObject(this, &ULMAWeaponComponent::OnNotifyReloadFinished);
			break;
		}
	}
}
//*****************************************************************************************************
void ULMAWeaponComponent::OnNotifyReloadFinished(USkeletalMeshComponent* SkeletalMesh)
{
	const auto Character = Cast<ACharacter>(GetOwner());
	if (Character->GetMesh() == SkeletalMesh)
	{
		AnimReloading = false;
	}
}
//*****************************************************************************************************
bool ULMAWeaponComponent::CanReload() const
{
	return !AnimReloading && Weapon->CanReload();
}
//*****************************************************************************************************
void ULMAWeaponComponent::ClipEmmpty() {
	if (!CanReload())
		return;
	NoFire(); // останавливаем стрельбу
	//UE_LOG(LogTemp, Display, TEXT("Bullets Reload111111111111111111111111"));
	Weapon->ChangeClip();
	AnimReloading = true;
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	Character->PlayAnimMontage(ReloadMontage);
}
//*****************************************************************************************************