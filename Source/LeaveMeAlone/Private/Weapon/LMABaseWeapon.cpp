
#include "Weapon/LMABaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
//#include "UGameplayStatics"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"


DEFINE_LOG_CATEGORY_STATIC(LogWeapon, All, All)
// Sets default values
//**********************************************************************************************************
ALMABaseWeapon::ALMABaseWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WeaponComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	SetRootComponent(WeaponComponent);
}
//**********************************************************************************************************
void ALMABaseWeapon::Fire() {
	if (!IsCurrentClipEmpty())
	{
		GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &ALMABaseWeapon::Shoot, AmmoWeapon.FireRate, true);
	}
}
	//**********************************************************************************************************
void ALMABaseWeapon::NoFire() {
	GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
}
//**********************************************************************************************************
void ALMABaseWeapon::ChangeClip()
{
	CurrentAmmoWeapon.Bullets = AmmoWeapon.Bullets;
}
//**********************************************************************************************************
void ALMABaseWeapon::Shoot() {
	//UE_LOG(LogTemp, Display, TEXT("Pif-Paf"));
	const FTransform SocketTransform = WeaponComponent->GetSocketTransform("Muzzle");
	const FVector TraceStart = SocketTransform.GetLocation();
	const FVector ShootDirection = SocketTransform.GetRotation().GetForwardVector();
	const FVector TraceEnd = TraceStart + ShootDirection * TraceDistance;
	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Black, false, 1.0f, 0, 2.0f);
	//UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootWave, TraceStart);
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);
	FVector TracerEnd = TraceEnd;
	if (HitResult.bBlockingHit)
	{
		MakeDamage(HitResult);
		TracerEnd = HitResult.ImpactPoint;
	}
	//if (HitResult.bBlockingHit)
	//{
	//	DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.0f, 24, FColor::Red, false, 1.0f);
	//}
	SpawnTrace(TraceStart, TracerEnd);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootWave, TraceStart);

	DecrementBullets();
	//	�������� ������������ ������ ����.2. ������ ����������� ����� �����������.3. �������� ����������� ��������,
	//		� ������ ������ ��� ���������� X ������ ������.4. �������� ����� ����� �����������.5. ����� ���������� ����� ����������� ��
	//			������������� ����������� �������� � DrawDebugLine,
	//		������� ����� ��������� ���������� :
	// ��������� �� ���
	// ����� ������ ���������� �����
	// �������� �����
	// ���� �����
	// ����� ����� ����������� �� ��������� ����� ��� ����� ��������
	// ������� ������� ����� ����� �����
	// ������� �����
	// ������� �����
	// LineTraceSingleByChannel � ������ ���������� �
	//				  ����������� ����� ����������� � ����� ������ ��������.������� ����� ��������� ��������� :
	// ��������� FHitResult � ������� �� ��� �������,
	//		��� ������ ��� ���������� � ����� �����������.
	// ��������� ����� ������
	// �������� ����� ������
	// ����� �������� � ������� �������� ��� ����� �����������, ��� ��������� ������� ����� �������������� 7. � � ����� ������ �� ��������,
	//		������ �� �� ���� - ����, ���� ������,
	//		�� ����� � ����� ��������� ������������� ����������� ������������.��� ����� �� ������������� �������� DrawDebugSphere,
	//		������� ��������� ��������� ��������� :
	// ��������� �� ���
	// ����� ����� �����
	// ������ ����������� �����
	// ���������� ��������� �����
	// ���� �����
	// ����� �� ����� ��������� ����� ����������
	// �����, ������� ����� ����������� �����
}

//**********************************************************************************************************
void ALMABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmoWeapon = AmmoWeapon;
}
//**********************************************************************************************************
void ALMABaseWeapon::DecrementBullets()
{
	if (!IsCurrentClipEmpty())
	{
		CurrentAmmoWeapon.Bullets--;
		//UE_LOG(LogWeapon, Display, TEXT("Bullets = %s"), *FString::FromInt(CurrentAmmoWeapon.Bullets));
	}
	if	(IsCurrentClipEmpty())
	{
		ClipEmpty.Broadcast();
	}
}
//**********************************************************************************************************
bool ALMABaseWeapon::IsCurrentClipEmpty() const
{
	return CurrentAmmoWeapon.Bullets == 0;
}
bool ALMABaseWeapon::IsClipFull() const
{
	return CurrentAmmoWeapon.Bullets == AmmoWeapon.Bullets;
}
//**********************************************************************************************************
bool ALMABaseWeapon::CanReload() const
{
	return !IsClipFull() && CurrentAmmoWeapon.Bullets < AmmoWeapon.Bullets;
}
//**********************************************************************************************************
void ALMABaseWeapon::SpawnTrace(const FVector& TraceStart, const FVector& TraceEnd)
{
	const auto TraceFX = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceEffect, TraceStart);
	if (TraceFX)
	{
		TraceFX->SetNiagaraVariableVec3(TraceName, TraceEnd);
	}
}
//**********************************************************************************************************
	void ALMABaseWeapon::MakeDamage(const FHitResult& HitResult)
	{
		const auto Zombie = HitResult.GetActor();
		if (!Zombie)
			return;
		const auto Pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (!Pawn)
			return;
		const auto Controller = Pawn->GetController<APlayerController>();
		if (!Controller)
			return;
		Zombie->TakeDamage(Damage, FDamageEvent(), Controller, this);
	}
//**********************************************************************************************************
// Called every frame
void ALMABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
//**********************************************************************************************************