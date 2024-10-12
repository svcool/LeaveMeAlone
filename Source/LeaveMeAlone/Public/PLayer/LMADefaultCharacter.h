// LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LMADefaultCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class LEAVEMEALONE_API ALMADefaultCharacter : public ACharacter
{
	GENERATED_BODY()

	
public:
	// Sets default values for this character's properties
	ALMADefaultCharacter();

protected:
	// Called when the game starts or when spawned
	

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



	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



private:

		float YRotation = -75.0f; //отвечает за поворот камеры по оси Y
		float ArmLength = 1400.0f; //отвечает за длину штатива
		float FOV = 55.0f; //отвечает за поле зрения камеры

		void MoveForward(float AxisValue); // будет отвечать за движение персонажа по оси X.
		void MoveRight(float AxisValue);   // будет отвечать за движение персонажа по оси Y.

		// управления зумом камеры 
		UPROPERTY(EditAnywhere, Category = "Camera")
		float MinZoomDistance = 300.0f;

		UPROPERTY(EditAnywhere, Category = "Camera")
		float MaxZoomDistance = 1400.0f;

		UPROPERTY(EditAnywhere, Category = "Camera")
		float ZoomSpeed = 100.0f;

		float CurrentZoomDistance;

		void MauseWheel(float AxisValue);  // Движение колеса мыши.
		
};
