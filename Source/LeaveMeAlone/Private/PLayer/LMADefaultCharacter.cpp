// LeaveMeAlone Game by Netologiya. All RightsReserved.


#include "Player/LMADefaultCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/LMAHealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h"


//**************************************************************************************************
// Sets default values
ALMADefaultCharacter::ALMADefaultCharacter()
{
 	
	PrimaryActorTick.bCanEverTick = true; //��������� ��� ������ (��������, ��������� ��� ������� � ����), ������� ��������� ��� ��������� ���������� ������ ����
	
	
	// ��������� �������
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent->SetupAttachment(GetRootComponent()); // ��������� ������������� ���������� ��� SpringArmComponent. RootComponent -��������� � �������� ����������� ������.  ��� ��������� ����������, ����� ��� Mesh, Camera, SpringArm � ������, ����� ���� ����������� � RootComponent ��� � ���� �����, �������� ��������.
	SpringArmComponent->SetUsingAbsoluteRotation(true); //������ ������� �� �������� ����� ������ �������������� � ������ �������� ���������.� ����� Top - Down Shooter, ������ ������ ��������� �������� ��� �������.
	SpringArmComponent->TargetArmLength = ArmLength;// �������� ����
	SpringArmComponent->SetRelativeRotation(FRotator(YRotation, 0.0f, 0.0f)); //��������� FRotator ������ ��������� � ��������� ������������������ : Pitch, Yaw, Roll.��� ��� ��� ���������� ���������� �������� �� ��� Y, �� ������������� Pitch ��������.
	SpringArmComponent->bDoCollisionTest = false; // ��������� ����������� ������ ��� ������������
	SpringArmComponent->bEnableCameraLag = true; //����������� ������, �������� �������
	SpringArmComponent->CameraLagSpeed = 3.0f; // �������� ������� 

	// ��������� ���������� �������� ���� 
	CurrentZoomDistance = SpringArmComponent->TargetArmLength;

	// ��������� ������
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->SetFieldOfView(FOV); //�������� �� ���� ������ ������
	CameraComponent->bUsePawnControlRotation = false;// ������ ������� ��������� ������ ��������� ������������ SpringArmComponent.
	
	
	//������ ������� � ������� ������
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//������� ��������� ��������
	HealthComponent = CreateDefaultSubobject<ULMAHealthComponent>("HealthComponent");

}
//**************************************************************************************************
// Called when the game starts or when spawned
void ALMADefaultCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (CursorMaterial)
	{
		CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector(0));
	}

	OnHealthChanged(HealthComponent->GetHealth());
	HealthComponent->OnDeath.AddUObject(this, &ALMADefaultCharacter::OnDeath); //�������� �� ������� ������
	HealthComponent->OnHealthChanged.AddUObject(this, &ALMADefaultCharacter::OnHealthChanged); //�������� �� ������� ��������� ��������
}
//**************************************************************************************************
// Called every frame
void ALMADefaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
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
	
	PlayerInputComponent->BindAxis("MoveForward", this, &ALMADefaultCharacter::MoveForward); // "MoveForward" � ��������� ������� Engine->Input: Axis
	PlayerInputComponent->BindAxis("MoveRight", this, &ALMADefaultCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Wheel", this, &ALMADefaultCharacter::MauseWheel); // ������ ����
}
//**************************************************************************************************
void ALMADefaultCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector(), AxisValue);
	//AddMovementInput � ��� ����������� �������, ������� � �������� ����������
	//����� ����������� �������� � ��������, �� ������� ����� �������� �����������.
}
//**************************************************************************************************
void ALMADefaultCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector(), AxisValue);
}
//**************************************************************************************************
void ALMADefaultCharacter::MauseWheel(float AxisValue)
{
	CurrentZoomDistance = CurrentZoomDistance - AxisValue * ZoomSpeed; // ��������� ����
		// �������� �� ����������� ��������
		if (CurrentZoomDistance < MinZoomDistance)
		{
			CurrentZoomDistance = MinZoomDistance;
		}

		// �������� �� ������������ ��������
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

	PlayAnimMontage(DeathMontage); //����� ������������ �������� �������

	GetCharacterMovement()->DisableMovement(); //��������� ��������

	SetLifeSpan(5.0f); //������������ ������� ����� 5 ���

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