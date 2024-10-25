// LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LMAHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath) //делегат смерти персонажа
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float); // для изменения здоровья пикап

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEAVEMEALONE_API ULMAHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	

public:	
	// Sets default values for this component's properties
	ULMAHealthComponent();

	UFUNCTION(BlueprintCallable)
	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable)
	bool IsDead() const;

	bool AddHealth(float NewHealth); //будет прибавлять нам количество жизней и возвращатьс	значение правды, если здоровье было успешно прибавлено.(pickup)
	bool IsHealthFull() const; //  вспомогательная функция, которая проверит, полное ли у нас здоровье в 	настоящее время.(pickup)
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDeath OnDeath; //делегат смерти
	FOnHealthChanged OnHealthChanged;
	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxHealth = 100.0f; //максимальное здоровье
	
	virtual void BeginPlay() override;

private:
	float Health = 0.0f; // текущее здоровье

	UFUNCTION() //(Engine\Source\Runtime\Engine\Classes\GameFramework\Actor.h), является DYNAMIC_MULTICAST, а значит мы обязаны созданную функцию пометить макросом UFUNCTION.
	void OnTakeAnyDamage(
		AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
//	Оставшиеся параметры :
//Актор, которому был нанесен ущерб.
//Количественная характеристика ущерба.
//Тип ущерба.
//Контроллер, который ответственный за ущерб.
//Актор, ответственный за ущерб.

};
