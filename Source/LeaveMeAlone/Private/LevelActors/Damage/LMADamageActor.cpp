// LeaveMeAlone Game by Netologiya. All RightsReserved.


#include "LevelActors/Damage/LMADamageActor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
//********************************************************************************************************************
// Sets default values
ALMADamageActor::ALMADamageActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// иницилизация компонентов
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetSphereRadius(SphereRadius);
	SetRootComponent(SphereComponent);
	//иницилизация меш
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(SphereComponent);
	StaticMesh->SetRelativeScale3D(FVector(2.0f, 2.0f, 0.05f));
	StaticMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -4.0f));
}
//********************************************************************************************************************
// Called when the game starts or when spawned
void ALMADamageActor::BeginPlay()
{
	Super::BeginPlay();
	
}
//********************************************************************************************************************
// Called every frame
void ALMADamageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), SphereRadius, nullptr, {}, this, nullptr, false);
//	Данная функция принимает следующие аргументы :
//Указатель на мир.
//Количественная характеристика урона.
//Центр сферы урона.
//Радиус сферы.
//Тип урона.
//Массив Акторов, которым мы не хотим наносить урон.
//Актор который непосредственно наносит ущерб
//Контроллер Актора, который наносит ущерб
//Как будет наноситься ущерб.Если правда – то урон во всей сфере одинаковый, если ложь то урон будет интерполироваться относительно центра сферы.
//Какие коллизии блокируют урон.
}
//********************************************************************************************************************
