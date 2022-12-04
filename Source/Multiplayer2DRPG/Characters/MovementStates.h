#pragma once

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	MS_Idle UMETA(DisplayName = "Idle"),
	MS_RunRight UMETA(DisplayName = "RunRight"),
	MS_RunLeft UMETA(DisplayName = "RunLeft"),
	MS_Jump UMETA(DisplayName = "Jump"),
	MS_Falling UMETA(DisplayName = "Falling"),

	MS_MAX UMETA(DisplayName = "DefaultMAX"), 
};