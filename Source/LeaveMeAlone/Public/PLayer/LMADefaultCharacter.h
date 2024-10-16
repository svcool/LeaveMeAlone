// LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LMADefaultCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ULMAHealthComponent;
class UAnimMontage; //������������ ������ ��� ������

UCLASS()
class LEAVEMEALONE_API ALMADefaultCharacter : public ACharacter
{
	GENERATED_BODY()

	
public:
	// Sets default values for this character's properties
	ALMADefaultCharacter();

	UFUNCTION()
	ULMAHealthComponent* GetHealthComponent() const { return HealthComponent; } //�������� ������� ������� 

protected:
	// Called when the game starts or when spawned
	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")// �������� �� ��������� ������
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components") // ��� ��������������� ���������� ���������� ������ � ���������, ����� ��� ���������� ��������
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
	UAnimMontage* DeathMontage; //��������� �� ��������

	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



private:

		float YRotation = -75.0f; //�������� �� ������� ������ �� ��� Y
		float ArmLength = 1400.0f; //�������� �� ����� �������
		float FOV = 55.0f; //�������� �� ���� ������ ������

		void MoveForward(float AxisValue); // ����� �������� �� �������� ��������� �� ��� X.
		void MoveRight(float AxisValue);   // ����� �������� �� �������� ��������� �� ��� Y.

		// ���������� ����� ������ 
		UPROPERTY(EditAnywhere, Category = "Camera")
		float MinZoomDistance = 300.0f;

		UPROPERTY(EditAnywhere, Category = "Camera")
		float MaxZoomDistance = 1400.0f;

		UPROPERTY(EditAnywhere, Category = "Camera")
		float ZoomSpeed = 100.0f;

		float CurrentZoomDistance;

		void MauseWheel(float AxisValue);  // �������� ������ ����.

		//�������� � ������
		void OnDeath();
		void OnHealthChanged(float NewHealth);

		void RotationPlayerOnCursor();// ������ �������� �������� �� ��������
		
};
