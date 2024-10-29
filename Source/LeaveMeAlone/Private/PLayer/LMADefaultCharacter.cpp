// LeaveMeAlone Game by Netologiya. All RightsReserved.


#include "Player/LMADefaultCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/LMAHealthComponent.h"
#include "Components/LMAWeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Engine/Engine.h"


//**************************************************************************************************
// Sets default values
ALMADefaultCharacter::ALMADefaultCharacter()
{
 	
	PrimaryActorTick.bCanEverTick = true; //настройка для актора (например, персонажа или объекта в игре), которая позволяет ему выполнять обновления каждый кадр
	
	
	// компонент штатива
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent->SetupAttachment(GetRootComponent()); // установка родительского компонента для SpringArmComponent. RootComponent -компонент в иерархии компонентов актора.  Все остальные компоненты, такие как Mesh, Camera, SpringArm и другие, могут быть прикреплены к RootComponent или к друг другу, формируя иерархию.
	SpringArmComponent->SetUsingAbsoluteRotation(true); //данное условие не позволит нашей камере поворачиваться в момент поворота персонажа.В жанре Top - Down Shooter, камера обычно находится статично над игроком.
	SpringArmComponent->TargetArmLength = ArmLength;// значение зума
	SpringArmComponent->SetRelativeRotation(FRotator(YRotation, 0.0f, 0.0f)); //структура FRotator хранит аргументы в следующей последовательности : Pitch, Yaw, Roll.Так как нам необходимо определить значения по оси Y, мы устанавливаем Pitch аргумент.
	SpringArmComponent->bDoCollisionTest = false; // отключаем возвращение камеры при столкновении
	SpringArmComponent->bEnableCameraLag = true; //сглаживание камеры, включает инерцию
	SpringArmComponent->CameraLagSpeed = 3.0f; // скорость инерции 

	// Установка начального значения зума 
	CurrentZoomDistance = SpringArmComponent->TargetArmLength;

	// компонент камеры
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->SetFieldOfView(FOV); //отвечает за поле зрения камеры
	CameraComponent->bUsePawnControlRotation = false;// данное условие запрещаем камере вращаться относительно SpringArmComponent.
	
	
	//запрет поворат в сторону камеры
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	//создаем компонент здоровья
	HealthComponent = CreateDefaultSubobject<ULMAHealthComponent>("HealthComponent");

	// создаем компонент оружие
	WeaponComponent = CreateDefaultSubobject<ULMAWeaponComponent>("Weapon");

	bIsSprinting = false;
	
}
//**************************************************************************************************
bool ALMADefaultCharacter::IsSprinting() const
{
	return bIsSprinting;
}
//**************************************************************************************************
void ALMADefaultCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (CursorMaterial)
	{
		CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector(0));
	}

	OnHealthChanged(HealthComponent->GetHealth());
	HealthComponent->OnDeath.AddDynamic(this, &ALMADefaultCharacter::OnDeath); //подписка на делегат смерти
	//HealthComponent->OnHealthChanged.AddUObject(this, &ALMADefaultCharacter::OnHealthChanged); //подписка на делегат изменения здоровья
}
//**************************************************************************************************
// Called every frame
void ALMADefaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	ControlStamina();

	if (!(HealthComponent->IsDead()))
	{
		RotationPlayerOnCursor();
	}

}
//**************************************************************************************************
// Called to bind functionality to input
void ALMADefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ALMADefaultCharacter::MoveForward); // "MoveForward" в настройка проекта Engine->Input: Axis
	PlayerInputComponent->BindAxis("MoveRight", this, &ALMADefaultCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Wheel", this, &ALMADefaultCharacter::MauseWheel); // колесо мыши
	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &ULMAWeaponComponent::Fire);//выстрел(левая кн. мышки)
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &ULMAWeaponComponent::NoFire); // выстрел(левая кн. мышки)
	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &ULMAWeaponComponent::Reload);	
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ALMADefaultCharacter::BeginSprint);		   // перезарядка
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ALMADefaultCharacter::EndSprint);		// перезарядка

}
//**************************************************************************************************
void ALMADefaultCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector(), AxisValue);
	//AddMovementInput – это стандартная функция, которая в качестве параметров
	//берет направление движения и величину, на которую будет умножено направление.
}
//**************************************************************************************************
void ALMADefaultCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector(), AxisValue);
}
//**************************************************************************************************
void ALMADefaultCharacter::MauseWheel(float AxisValue)
{
	CurrentZoomDistance = CurrentZoomDistance - AxisValue * ZoomSpeed; // Изменение зума
		// Проверка на минимальное значение
		if (CurrentZoomDistance < MinZoomDistance)
		{
			CurrentZoomDistance = MinZoomDistance;
		}

		// Проверка на максимальное значение
		if (CurrentZoomDistance > MaxZoomDistance)
		{
			CurrentZoomDistance = MaxZoomDistance;
		}
		SpringArmComponent->TargetArmLength = CurrentZoomDistance;
}
//**************************************************************************************************
void ALMADefaultCharacter::OnDeath()
{
	CurrentCursor->DestroyRenderState_Concurrent();

	PlayAnimMontage(DeathMontage); //вызов проигрывания анимации монтажа

	GetCharacterMovement()->DisableMovement(); //отключаем движение

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ALMADefaultCharacter::DestroyWeaponComponent, 4.8f, false); // переопределен для уничтожения оружия
	SetLifeSpan(5.0f); //унитчтожение объекта через 5 сек
	
	//WeaponComponent->DestroyComponent();
	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
}
//**************************************************************************************************
void ALMADefaultCharacter::OnHealthChanged(float NewHealth)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Health = %f"), NewHealth));
}
//**************************************************************************************************
void ALMADefaultCharacter::RotationPlayerOnCursor()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		FHitResult ResultHit;
		PC->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);
		float FindRotatorResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ResultHit.Location).Yaw;
		SetActorRotation(FQuat(FRotator(0.0f, FindRotatorResultYaw, 0.0f)));
		if (CurrentCursor)
		{
			CurrentCursor->SetWorldLocation(ResultHit.Location);
		}
	}
}
//**************************************************************************************************
void ALMADefaultCharacter::BeginSprint() {
	if (Stamina >= 50 && IsMovingForward())
	{
		
		bIsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = 900.0f;
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("SprintEnd"));
	}
	}
//**************************************************************************************************
void ALMADefaultCharacter::EndSprint() {
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("SprintEnd"));
}
//**************************************************************************************************
void ALMADefaultCharacter::ControlStamina() {
	if (bIsSprinting){
		Stamina = Stamina - SprintCost;
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Sprint = %f"), Stamina));
		if (Stamina <=0){
			EndSprint();
		}
	}
	else
	{
			Stamina = Stamina + StaminaRecoveryRate / 10; // Восстановление выносливости
			Stamina = FMath ::Min(Stamina, MaxStamina);
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("RecoveryStamina = %f"), Stamina));
		}
}
//**************************************************************************************************
bool ALMADefaultCharacter::IsMovingForward()
{
	// Получаем вектор направления движения
	FVector Velocity = GetVelocity();
	FVector ForwardVector = GetActorForwardVector();

	// Проверяем, движется ли персонаж вперед (по оси Y)
	return FVector::DotProduct(Velocity, ForwardVector) > 0;
}
//**************************************************************************************************
void ALMADefaultCharacter::DestroyWeaponComponent()
{
	if (WeaponComponent)
	{
		// Уничтожить компонент оружия
		WeaponComponent->DestroyComponent();
	}

}
//**************************************************************************************************