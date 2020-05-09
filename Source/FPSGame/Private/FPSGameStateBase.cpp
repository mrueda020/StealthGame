// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameStateBase.h"
#include "UObject/UObjectIterator.h"
#include "FPSPlayerController.h"

//We disable the input for each controlled pawn
void AFPSGameStateBase::MulticastOnMissionComplete_Implementation(APawn* InstigatorPawn, bool bIsMissionSuccessful)
{
	for (TObjectIterator<APlayerController>Itr; Itr; ++Itr)
	{
		APawn* Pawn = Itr->GetPawn();
		
		if (Pawn && Pawn->IsLocallyControlled())
		{
			AFPSPlayerController* PC = Cast<AFPSPlayerController>(Pawn->GetController());
			if (PC)
			{
				PC->OnMissionCompleted(InstigatorPawn, bIsMissionSuccessful);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("APlayerController cast fails"));
			}
			Pawn->DisableInput(nullptr);
		}
	}
}