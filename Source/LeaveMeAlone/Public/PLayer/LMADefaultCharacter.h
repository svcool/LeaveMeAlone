// LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LMADefaultCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ULMAHealthComponent;
class ULMAWeaponComponent;
class UAnimMontage; //анимационный монтаж для смерти

UCLASS()
class LEAVEMEALONE_API ALMADefaultCharacter : public ACharacter
{
	GENERATED_BODY()

	
public:
	// Sets default values for this character's properties
	ALMADefaultCharacter();

	UFUNCTION()
	ULMAHealthComponent* GetHealthComponent() const { return HealthComponent; } //получить текущее здровье 

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")// отвечает за компонент камеры
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components") // для автоматического управления поведением камеры в ситуациях, когда она становится закрытой
	UCameraComponent* CameraComponent;

	UPROPERTY()
	UDecalComponent* CurrentCursor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	UMaterialInterface* CursorMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	FVector CursorSize = FVector(20.0f, 40.0f, 40.0f);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Health")
	ULMAHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathMontage; //указатель на анимацию

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	ULMAWeaponComponent* WeaponComponent;// указатель на оружие

	/***********Satart_CameraZoom***********/
	UPROPERTY(EditAnywhere, Category = "Camera")
	float MinZoomDistance = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float MaxZoomDistance = 1400.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float ZoomSpeed = 100.0f;
	/***********End_CameraZoom***********/

	/***********Satart_Sprint***********/
	UPROPERTY(EditAnywhere, Category = "Sprint")
	float Stamina = 100.0f; // выносливость

	UPROPERTY(EditAnywhere, Category = "Sprint")
	float MaxStamina = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Sprint")
	float SprintCost = 0.3f;

	UPROPERTY(EditAnywhere, Category = "Sprint")
	float StaminaRecoveryRate = 1.3f; // Скорость восстановления выносливости

	UPROPERTY(BlueprintReadOnly, Category = "Sprint")
	bool bIsSprinting; // Флаг спринта

	UFUNCTION(BlueprintCallable)
	bool IsSprinting() const;

	/***********EndSprint***********/

	virtual void BeginPlay() override;


public:	

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



private:

		float YRotation = -75.0f; //отвечает за поворот камеры по оси Y
		float ArmLength = 1400.0f; //отвечает за длину штатива
		float FOV = 55.0f; //отвечает за поле зрения камеры

		void MoveForward(float AxisValue); // будет отвечать за движение персонажа по оси X.
		void MoveRight(float AxisValue);   // будет отвечать за движение персонажа по оси Y.

		float CurrentZoomDistance;

		void MauseWheel(float AxisValue);  // Движение колеса мыши.

		//здоровье и смерть
		void OnDeath();
		void OnHealthChanged(float NewHealth);

		void RotationPlayerOnCursor();// логика движения персонаж за курсором

		void BeginSprint();
		void EndSprint();
		void ControlStamina();
		bool IsMovingForward();

		
};
