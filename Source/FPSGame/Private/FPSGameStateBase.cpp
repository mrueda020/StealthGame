// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameStateBase.h"
#include "UObject/UObjectIterator.h"

void AFPSGameStateBase::MulticastOnMissionComplete_Implementation(APawn* InstigatorPawn, bool bIsMissionSuccessful)
{
	
	for (TObjectIterator<APawn> Itr; Itr; ++Itr)
	{
		if (Itr && Itr->IsLocallyControlled())
		{
			Itr->DisableInput(nullptr);
		}
	}
}