// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "FPSGameMode.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHeard);
	GuardState = EGuardState::Idle;

}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	OriginalRotation = GetActorRotation();
}

void AFPSAIGuard::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn == nullptr)
	{   
		UE_LOG(LogTemp, Warning, TEXT("Pawn not seen!"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Pawn seen!"));
	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 30.0f, 12, FColor::Red, false, 10.0f);

	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		UE_LOG(LogTemp, Log, TEXT("Mission failed!"));
		GM->CompleteMission(SeenPawn, false);
	}

	SetGuardState(EGuardState::Alerted);


}


void AFPSAIGuard::OnNoiseHeard(APawn* HeardInstigator, const FVector& Location, float Volume)
{	
	if (GuardState != EGuardState::Alerted)
	{
		DrawDebugSphere(GetWorld(), Location, 30.0f, 12, FColor::Green, false, 10.0f);
		FVector Direction = Location - GetActorLocation();
		Direction.Normalize();
		FRotator NewRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
		NewRotation.Pitch = 0.0f;
		NewRotation.Roll = 0.0f;
		SetActorRotation(NewRotation);
		GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
		GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AFPSAIGuard::ResetOrientation, 3.0f);
		SetGuardState(EGuardState::Suspicious);
	}
}


void AFPSAIGuard::ResetOrientation()
{
	if (GuardState != EGuardState::Alerted)
	{
		SetActorRotation(OriginalRotation);
		SetGuardState(EGuardState::Idle);
	}
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void  AFPSAIGuard::SetGuardState(EGuardState NewState)
{

	if (GuardState == NewState)
	{
		return;
	}

	GuardState = NewState;
	OnGuardStateChange(GuardState);
}
