// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "MovementStates.h"
#include "Net/UnrealNetwork.h"
#include "PaperFlipbookComponent.h"
#include "PaperSprite.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

ADefaultCharacter::ADefaultCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	GetCapsuleComponent()->SetCapsuleHalfHeight(36.f);
	GetCapsuleComponent()->SetCapsuleRadius(16.f);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->SetWorldRotation(FRotator(0.f, -90.f, 0.f));
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->ProjectionMode = ECameraProjectionMode::Orthographic;
	Camera->OrthoWidth = 1304.f;
	
	GetCharacterMovement()->GravityScale = 1.9f;
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

void ADefaultCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ADefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ADefaultCharacter::Jump);

	PlayerInputComponent->BindAxis("Move Right / Move Left", this, &ADefaultCharacter::MoveRightOrLeft);
}

void ADefaultCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ADefaultCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ADefaultCharacter::SetMovementState(EMovementState State)
{
	ServerSetMovementState(State);
}

void ADefaultCharacter::UpdateAnim(EMovementState State)
{
	switch (State)
	{
	case EMovementState::MS_Idle:
		if (FB_Idle)
		{
			GetSprite()->SetFlipbook(FB_Idle);
		}
		break;
	case EMovementState::MS_RunRight:
		if (FB_Run)
		{
			GetSprite()->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
			GetSprite()->SetFlipbook(FB_Run);
		}
		break;
	case EMovementState::MS_RunLeft:
		if (FB_Run)
		{
			GetSprite()->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
			GetSprite()->SetFlipbook(FB_Run);
		}
		break;
	case EMovementState::MS_Jump:
		if (FB_Jump)
			GetSprite()->SetFlipbook(FB_Jump);
		break;
	case EMovementState::MS_Falling:
		if (FB_Jump)
			GetSprite()->SetFlipbook(FB_Jump);
		break;
	default:
		if (FB_Idle)
			GetSprite()->SetFlipbook(FB_Idle);
		break;
	}
}

void ADefaultCharacter::MulticastSetMovementState_Implementation(EMovementState State)
{
	UpdateAnim(State);
}

void ADefaultCharacter::ServerSetMovementState_Implementation(EMovementState State)
{
	MovementState = State; 
	if (HasAuthority())
		MulticastSetMovementState(State); 
}

void ADefaultCharacter::MoveRightOrLeft(float Value)
{
	AddMovementInput(UKismetMathLibrary::GetForwardVector(GetControlRotation()), Value);
	if (GetCharacterMovement()->IsFalling())
	{
		if (GetVelocity().Z > 0.0f)
			SetMovementState(EMovementState::MS_Jump);
		else
			SetMovementState(EMovementState::MS_Falling);
	}
	else
	{
		if (Value == 0.0f)
			SetMovementState(EMovementState::MS_Idle);
		else if (Value > 0.0f)
			SetMovementState(EMovementState::MS_RunRight);
		else if (Value < 0.0f)
			SetMovementState(EMovementState::MS_RunLeft);
	}
}

void ADefaultCharacter::Jump()
{
	if (!GetCharacterMovement()->IsFalling())
	{
		Super::Jump();
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), JumpSound, GetActorLocation()); 
	}
}
