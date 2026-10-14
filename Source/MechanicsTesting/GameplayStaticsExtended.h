#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayStaticsExtended.generated.h"

UENUM(BlueprintType)
enum class EOscillateClampMode : uint8
{
	None = 0,
	Clamped = 1,
	Wrapped = 2
};

UCLASS()
class MECHANICSTESTING_API UGameplayStaticsExtended : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "Math|Oscillation")
	static float GetOscillation(float Alpha, float Frequency = 1.f, const bool bEqualized = true, const EOscillateClampMode ClampMode = EOscillateClampMode::Clamped);
	UFUNCTION(BlueprintPure, Category = "Math|Oscillation")
	static float GetOscillationDelta(float Alpha, float Delta = 0.f, float Frequency = 1.f, const bool bEqualized = true, const EOscillateClampMode ClampMode = EOscillateClampMode::Clamped);
	UFUNCTION(BlueprintCallable, Category = "Math|Oscillation", meta = (ReturnDisplayName = "Delta"))
	static float OscillateAdditive(UPARAM(ref)float& Time, float MaxTime, float DeltaTime = 0.f, float Frequency = 1.0f, const bool bEqualized = true, const EOscillateClampMode ClampMode = EOscillateClampMode::Clamped);

	UFUNCTION(BlueprintPure, Category = "Math|Oscillation")
	static float OscillateNormalizedDamped(float Alpha, float StepDown, float Frequency = 1.f, bool bEqualized = true, bool bClamped = true);

#pragma region Clamping
	UFUNCTION(BlueprintPure, Category = "Math|Clamping")
	static float ClampStep(const float Value, const float StepSize);
	UFUNCTION(BlueprintPure, Category = "Math|Clamping")
	static FVector ClampStep3D(const FVector Vector, const FVector StepSize);
	UFUNCTION(BlueprintPure, Category = "Math|Clamping")
	static FVector2D ClampStep2D(const FVector2D Vector, const FVector2D StepSize);
	UFUNCTION(BlueprintPure, Category = "Math|Clamping")
	static FRotator ClampStepRotation(const FRotator Rotation, const FRotator StepSize);
#pragma endregion

#pragma region Wrapping
	UFUNCTION(BlueprintPure, Category = "Math|Wrapping")
	static float WrapValue(float Value, float Min, float Max);
	UFUNCTION(BlueprintPure, Category = "Math|Wrapping")
	static FVector WrapVector(FVector Vector, FVector Min, FVector Max);
	UFUNCTION(BlueprintPure, Category = "Math|Wrapping")
	static FVector2D WrapVector2D(FVector2D Vector, FVector2D Min, FVector2D Max);
#pragma endregion

	UFUNCTION(BlueprintCallable)
	static void AddWorldRotationQuat(USceneComponent* Target, FVector Axis, float Degrees)
	{
		if (!IsValid(Target) || Axis.IsNearlyZero()) return;

		Target->AddWorldRotation(
			FQuat(Axis.GetSafeNormal(), FMath::DegreesToRadians(Degrees)));
	}
};