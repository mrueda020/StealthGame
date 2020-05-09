// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "FPSBlackHole.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "FPSGameStateBase.h"
#include "UObject/UObjectIterator.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();

	// set the GameStateClass 
	GameStateClass = AFPSGameStateBase::StaticClass();
	
}

void AFPSGameMode::CompleteMission(APawn* InstigatorPawn, bool bIsMissionSuccessful)
{	//Verify if InstigatorPawn exits
	if (InstigatorPawn)
	{
		if (SpectatorViewpointClass)
		{
			TArray<AActor*> ReturnedActors;
			UGameplayStatics::GetAllActorsOfClass(this, SpectatorViewpointClass,ReturnedActors);
			if (ReturnedActors.Num() > 0)
			{
				AActor* NewViewTarget = ReturnedActors[0];
				//We changed the view target for each controlled pawn
				for (TObjectIterator<AActor>Itr; Itr; ++Itr)
				{	
					APlayerController* PC = Cast<APlayerController>(Itr->GetInstigatorController());
					if (PC)
					{
						PC->SetViewTargetWithBlend(NewViewTarget, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
					}
					else
					{	
						UE_LOG(LogTemp, Warning, TEXT("PlayerController cast failed"));
					}
						
				}
				
				/**if (BlackHoleClass)
				{
					FActorSpawnParameters ActorSpawnParams;
					ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
					GetWorld()->SpawnActor<AFPSBlackHole>(BlackHoleClass, FVector(-600.0f, -80.0f, 1740.0f), FRotator(0), ActorSpawnParams);
					UE_LOG(LogTemp,Warning,TEXT("BlackHole should appear!!"));
				}*/
				
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid Actors"));

		}
	}

	AFPSGameStateBase* GS = GetGameState<AFPSGameStateBase>();
	if (GS)
	{
		GS->MulticastOnMissionComplete(InstigatorPawn, bIsMissionSuccessful);
	}
	//This function belongs to the FPSPlayerController class
	OnMissionCompleted(InstigatorPawn, bIsMissionSuccessful);
}
