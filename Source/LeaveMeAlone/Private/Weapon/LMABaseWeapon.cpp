
#include "Weapon/LMABaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "TimerManager.h"

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
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Black, false, 1.0f, 0, 2.0f);
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);
	if (HitResult.bBlockingHit)
	{
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.0f, 24, FColor::Red, false, 1.0f);
	}
	DecrementBullets();
	//	Получили расположение сокета дула.2. Задали изначальную точку трассировки.3. Получили направление стрельбы,
	//		в данном случае это координата X нашего сокета.4. Получили точку конца трассировки.5. Чтобы нарисовать линию трассировки мы
	//			воспользуемся специальной функцией – DrawDebugLine,
	//		которая имеет несколько аргументов :
	// Указатель на мир
	// Точка старта прорисовки линии
	// Конечная точка
	// Цвет линии
	// Будет линии прорисована на постоянно время или будет исчезать
	// Сколько времени линии будет видна
	// Глубина линии
	// Толщина линии
	// LineTraceSingleByChannel – вернет информацию и
	//				  пересечении линии трассировки с самым первым объектом.Функция имеет следующие аргументы :
	// Структура FHitResult с которой мы уже знакомы,
	//		она хранит всю информацию о точке пересечения.
	// Начальная точка трейса
	// Конечная точка трейса
	// Канал коллизии с которым работает наш канал трассировки, все остальные объекты будут игнорироваться 7. И в конце концов мы проверим,
	//		попали ли мы куда - либо, если попали,
	//		то будем в месте попадания прорисовывать сферическое пространство.Для этого мы воспользуемся функцией DrawDebugSphere,
	//		которая принимаем следующие аргументы :
	// Указатель на мир
	// Центр точки сферы
	// Радиус создаваемой сферы
	// Количество сегментов сферы
	// Цвет сферы
	// Будет ли сфера оставлена после прорисовки
	// Время, которое будет прорисована сферы
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
// Called every frame
void ALMABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
//**********************************************************************************************************