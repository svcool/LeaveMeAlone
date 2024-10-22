// LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LMAHealthPickup.generated.h"

class USphereComponent;
class ALMADefaultCharacter;

UCLASS()
class LEAVEMEALONE_API ALMAHealthPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALMAHealthPickup();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Pickup")
	USphereComponent* SphereComponent; // сфера для взаимодействия

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	float RespawnTime = 5.0f; //возрождение объекта Pickup

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "5.0", ClampMax = "100.0")) //ограничения мин=50здоровья
	float HealthFromPickup = 100.0f; //здоровье для возрождения у персонажа

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override; //Метод, который вызывается, когда другой актер начинает пересекаться с этим объектом.для обработки логики взаимодействия, например, когда игрок подбирает объект
	 
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool GivePickup(ALMADefaultCharacter* Character); // будет отвечать за то, взяли ли мы пикап.
	void PickupWasTaken(); // будет запускать таймер респауна и скрывать наш пикап на временной промежуток, после взятия пикапа.
	void RespawnPickup(); //будет 	возвращать видимость пикапа по прошествии определенного количества времени.

};
