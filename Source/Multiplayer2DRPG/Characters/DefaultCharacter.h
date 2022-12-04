// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "DefaultCharacter.generated.h"

class USoundCue;
class UPaperFlipbook;
enum class EMovementState : uint8;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class MULTIPLAYER2DRPG_API ADefaultCharacter : public APaperCharacter
{
	GENERATED_BODY()
public:
	ADefaultCharacter();
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(VisibleAnywhere, Category = Camera) 
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;
	
	UPROPERTY(EditDefaultsOnly, Category = "Anims")
	UPaperFlipbook* FB_Idle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Anims")
	UPaperFlipbook* FB_Run;

	UPROPERTY(EditDefaultsOnly, Category = "Anims")
	UPaperFlipbook* FB_Jump;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* JumpSound; 
protected:
	virtual void BeginPlay() override;

	// Movement 
	void MoveRightOrLeft(float Value); 

	virtual void Jump();

	UPROPERTY(VisibleAnywhere, Category = "Movement") 
	EMovementState MovementState;
	
	void SetMovementState(EMovementState State);
	UFUNCTION(Server, Reliable)
	void ServerSetMovementState(EMovementState State);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetMovementState(EMovementState State); 

	void UpdateAnim(EMovementState State);
public:
};
