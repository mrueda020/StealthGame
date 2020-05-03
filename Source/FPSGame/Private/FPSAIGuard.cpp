// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "FPSGameMode.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

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
	CurrentTargetPoint = TargetPoints[0];
	if (bPatrol)
	{
		MoveToNextPatrolPoint();
	}
}

void AFPSAIGuard::OnPawnSeen(APawn* SeenPawn)
{	//We check is the player pawn exists and is seen by the guard
	if (SeenPawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn not seen!"));
		return;
	}
	//If the Guard sees the player
	UE_LOG(LogTemp, Warning, TEXT("Pawn seen!"));
	// We draw a sphere in the SeenPawn Location
	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 30.0f, 12, FColor::Red, false, 10.0f);
	//The GuardStae change to alerted
	SetGuardState(EGuardState::Alerted);
	//The guard stops moving
	auto* Controller = GetController();
	if (Controller)
	{
		Controller->StopMovement();
	}
	
	//The mission is finished and is gameover 
	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		UE_LOG(LogTemp, Log, TEXT("Mission failed!"));
		//The mission was not successful
		GM->CompleteMission(SeenPawn, false);
	}

	

}


void AFPSAIGuard::OnNoiseHeard(APawn* HeardInstigator, const FVector& Location, float Volume)
{	
	//If the guard is not alerted
	if (GuardState != EGuardState::Alerted)
	{
		//We draw a sphere in the posicion where the guard hearded a noise
		DrawDebugSphere(GetWorld(), Location, 30.0f, 12, FColor::Green, false, 10.0f);
		//We rotated the guard to the direction of the noise
		//We obtained the direction of the noise
		FVector Direction = Location - GetActorLocation();
		Direction.Normalize();
		//Calculate the new rotation for the guard
		FRotator NewRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
		NewRotation.Pitch = 0.0f;
		NewRotation.Roll = 0.0f;
		SetActorRotation(NewRotation);
		GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
		GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AFPSAIGuard::ResetOrientation, 3.0f);
		SetGuardState(EGuardState::Suspicious);
		auto* Controller = GetController();
		if (Controller)
		{
			Controller->StopMovement();
		}
	}
}

//Resets the GuardState and continue the Patrol 
void AFPSAIGuard::ResetOrientation()
{
	if (GuardState != EGuardState::Alerted)
	{
		SetActorRotation(OriginalRotation);
		SetGuardState(EGuardState::Idle);

		if (bPatrol)
			MoveToNextPatrolPoint();

	}
}

//Iterate the target points of the guard patrol road
void AFPSAIGuard::MoveToNextPatrolPoint()
{
	int32 TotalPatrolPoints = TargetPoints.Num();

	for (int32 i = 0; i<TotalPatrolPoints;i++)
	{
		if (TargetPoints[i] == CurrentTargetPoint)
		{
	
			if (i < TotalPatrolPoints-1)
			{
				CurrentTargetPoint = TargetPoints[i+1];
				break;
			}
			else
			{
				CurrentTargetPoint = TargetPoints[0];
				break;
			}

		}
	}

	//UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), GetWorld()->GetFirstPlayerController()->GetPawn());
	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentTargetPoint);
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrentTargetPoint == nullptr)
		UE_LOG(LogTemp, Warning, TEXT("NO TargetPoint!!"));

	if (CurrentTargetPoint)
	{
		FVector CurrentPosition = GetActorLocation() - CurrentTargetPoint->GetActorLocation();
		float distance = CurrentPosition.Size();
		UE_LOG(LogTemp, Warning, TEXT("The distance between the targetpoints and the guard is  %f"), distance);
		if (distance < 110)
		{
			MoveToNextPatrolPoint();
		}
	}
	
	/*if (bPatrol)
	{
		FVector TargetDistance = TargetPoints[0]->GetActorLocation() - TargetPoints[1]->GetActorLocation();
		float x = TargetDistance.Size();
		UE_LOG(LogTemp, Warning, TEXT("The distance between targetpoints is  %f"), x);

	}*/
	

}

//Set Guard new state
void  AFPSAIGuard::SetGuardState(EGuardState NewState)
{
	if (GuardState == NewState)
	{
		return;
	}
	GuardState = NewState;
	OnGuardStateChange(GuardState);

}



