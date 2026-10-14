#include "GameplayStaticsExtended.h"

#define STEP 0.01f

#pragma region Oscillation
float UGameplayStaticsExtended::GetOscillation(float Alpha, float Frequency, const bool bEqualized, const EOscillateClampMode ClampMode)
{
	if (Frequency <= 0.f) return 0.f;

	switch (ClampMode)
	{
	case EOscillateClampMode::Clamped: Alpha = FMath::Clamp(Alpha, 0.f, 1.f);
		break;
	case EOscillateClampMode::Wrapped:
		Alpha = WrapValue(Alpha, 0.f, 1.f);
		Frequency = FMath::RoundToInt(Frequency);
		break;
	}

	return (bEqualized) ?
		ClampStep(FMath::Sin(Alpha * FMath::RoundToInt(Frequency) * 2.f * PI), STEP) :
		ClampStep(0.5f - 0.5f * FMath::Cos(Alpha * Frequency * 2.f * PI), STEP);
}

float UGameplayStaticsExtended::GetOscillationDelta(float Alpha, float Delta, float Frequency, const bool bEqualized, const EOscillateClampMode ClampMode)
{
	return ClampStep(
		GetOscillation(Alpha, Frequency, bEqualized, ClampMode) -
		GetOscillation(Alpha - Delta, Frequency, bEqualized, ClampMode),
		STEP);
}

float UGameplayStaticsExtended::OscillateAdditive(UPARAM(ref) float& Time, float MaxTime, float DeltaTime, float Frequency, const bool bEqualized, const EOscillateClampMode ClampMode)
{
	if (MaxTime <= 0.f || Time >= MaxTime) return 0.f;

	const float PreviousTime = Time;
	const float ActualDeltaTime = FMath::Max(
		FMath::Min(Time + DeltaTime, MaxTime) - PreviousTime, 0);

	return GetOscillationDelta(Time / MaxTime, ActualDeltaTime / MaxTime,
		Frequency, bEqualized, ClampMode);
}

float UGameplayStaticsExtended::OscillateNormalizedDamped(float Alpha, float StepDown,
	float Frequency, bool bEqualized, bool bClamped)
{
	const float Freq = bEqualized ? FMath::RoundToInt(Frequency) : Frequency;
	const float CycleIndex = FMath::FloorToFloat(Alpha * Freq);
	const float Falloff = FMath::Max(1.f - CycleIndex * StepDown, 0.f);

	//return Oscillate(Alpha, Frequency, bEqualized, bClamped) * Falloff;
	return 0.f;
}
#pragma endregion

#pragma region Clamping
float UGameplayStaticsExtended::ClampStep(const float Value, const float StepSize)
{
	return (StepSize > 0.0f) ? (float)FMath::RoundToInt(Value / StepSize) * StepSize : 0.f;
}

FVector UGameplayStaticsExtended::ClampStep3D(const FVector Vector, const FVector StepSize)
{
	return FVector(ClampStep(Vector.X, StepSize.X),
		ClampStep(Vector.Y, StepSize.Y),
		ClampStep(Vector.Z, StepSize.Z));
}

FVector2D UGameplayStaticsExtended::ClampStep2D(const FVector2D Vector, const FVector2D StepSize)
{
	return FVector2D(ClampStep(Vector.X, StepSize.X),
		ClampStep(Vector.Y, StepSize.Y));
}

FRotator UGameplayStaticsExtended::ClampStepRotation(const FRotator Rotation, const FRotator StepSize)
{
	return FRotator(ClampStep(Rotation.Pitch, StepSize.Pitch),
		ClampStep(Rotation.Yaw, StepSize.Yaw),
		ClampStep(Rotation.Roll, StepSize.Roll));
}

float UGameplayStaticsExtended::WrapValue(float Value, float Min, float Max)
{
	const float Range = Max - Min;
	return FMath::Fmod(FMath::Fmod(Value - Min, Range) + Range, Range) + Min;
}

FVector UGameplayStaticsExtended::WrapVector(FVector Vector, FVector Min, FVector Max)
{
	return FVector();
}

FVector2D UGameplayStaticsExtended::WrapVector2D(FVector2D Vector, FVector2D Min, FVector2D Max)
{
	return FVector2D();
}
#pragma endregion