// LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LMAHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath) //������� ������ ���������
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float); // ��� ��������� �������� �����

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

	bool AddHealth(float NewHealth); //����� ���������� ��� ���������� ������ � �����������	�������� ������, ���� �������� ���� ������� ����������.(pickup)
	bool IsHealthFull() const; //  ��������������� �������, ������� ��������, ������ �� � ��� �������� � 	��������� �����.(pickup)
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDeath OnDeath; //������� ������
	FOnHealthChanged OnHealthChanged;
	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxHealth = 100.0f; //������������ ��������
	
	virtual void BeginPlay() override;

private:
	float Health = 0.0f; // ������� ��������

	UFUNCTION() //(Engine\Source\Runtime\Engine\Classes\GameFramework\Actor.h), �������� DYNAMIC_MULTICAST, � ������ �� ������� ��������� ������� �������� �������� UFUNCTION.
	void OnTakeAnyDamage(
		AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
//	���������� ��������� :
//�����, �������� ��� ������� �����.
//�������������� �������������� ������.
//��� ������.
//����������, ������� ������������� �� �����.
//�����, ������������� �� �����.

};
