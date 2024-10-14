// LeaveMeAlone Game by Netologiya. All RightsReserved.


#include "LevelActors/Pickup/LMAHealthPickup.h"
#include "Components/LMAHealthComponent.h"
#include "Components/SphereComponent.h"
#include "Player/LMADefaultCharacter.h"
//**************************************************************************************************
// Sets default values
ALMAHealthPickup::ALMAHealthPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent"); //создает новый компонент сферы (USphereComponent) и регистрирует его как часть объекта.
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly); //компонент будет обрабатывать столкновения.
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);//устанавливает реакцию компонента на столкновения для всех каналов.ECR_Overlap указывает, что компонент будет реагировать на столкновения путем наложения (overlap), а не блокировки или игнорирования.
	SetRootComponent(SphereComponent);

}
//**************************************************************************************************
// Called when the game starts or when spawned
void ALMAHealthPickup::BeginPlay()
{
	Super::BeginPlay();
	
}
//**************************************************************************************************
void ALMAHealthPickup::NotifyActorBeginOverlap(AActor* OtherActor)
{	//отработает, при пересечении персонажем капсулы пикапы
	Super::NotifyActorBeginOverlap(OtherActor); //вызывает реализацию метода NotifyActorBeginOverlap в базовом классе. 
	const auto Charcter = Cast<ALMADefaultCharacter>(OtherActor); //Cast безопасно проверяет, является ли OtherActor экземпляром или подклассом ALMADefaultCharacter. Если приведение успешно, Charcter будет указывать на объект типа ALMADefaultCharacter. Если нет, Charcter будет nullptr
	if (GivePickup(Charcter))
	{
		PickupWasTaken();
	}
}
//**************************************************************************************************
// Called every frame
void ALMAHealthPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
//**************************************************************************************************
bool ALMAHealthPickup::GivePickup(ALMADefaultCharacter* Character)
{// прибавить количество здоровья из пикапа в компонент здоровья
	const auto HealthComponent = Character->GetHealthComponent();
	if (!HealthComponent)
		return false;

	return HealthComponent->AddHealth(HealthFromPickup);
}
//**************************************************************************************************
void ALMAHealthPickup::PickupWasTaken()
{
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);//отключаем столкновения
	GetRootComponent()->SetVisibility(false, true); // скрытие объекта после подбора

	FTimerHandle RespawnTimerHandle; //тамймер возрождения пикапа
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ALMAHealthPickup::RespawnPickup, RespawnTime); //после истечения таймера вызов функции RespawnPickup()
}
//**************************************************************************************************
void ALMAHealthPickup::RespawnPickup()
{//возвращаем пикап на место
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	GetRootComponent()->SetVisibility(true, true);
}
//**************************************************************************************************