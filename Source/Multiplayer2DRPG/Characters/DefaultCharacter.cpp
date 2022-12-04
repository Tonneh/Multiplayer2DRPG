// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

ADefaultCharacter::ADefaultCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName); 
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


void ADefaultCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ADefaultCharacter::MoveRightOrLeft(float Value)
{
	
	AddMovementInput(UKismetMathLibrary::GetForwardVector(GetControlRotation()), Value); 
}

void ADefaultCharacter::Jump()
{
	if (!GetCharacterMovement()->IsFalling())
		Super::Jump();
}