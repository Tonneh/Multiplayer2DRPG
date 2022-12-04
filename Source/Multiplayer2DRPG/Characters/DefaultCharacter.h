// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "DefaultCharacter.generated.h"

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
private:
	UPROPERTY(VisibleAnywhere, Category = Camera) 
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;

protected:
	virtual void BeginPlay() override;

	// Movement 
	void MoveRightOrLeft(float Value); 

	virtual void Jump(); 
public:
};
