// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameMode.generated.h"

class AFPSBlackHole;

UCLASS()
class AFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AFPSGameMode();

	void CompleteMission(APawn* InstigatorPawn,bool bIsMissionSuccessful);


	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
		void OnMissionCompleted(APawn* InstigatorPawn, bool bIsMissionSuccessful);


protected:
	UPROPERTY(EditDefaultsOnly,Category = "Spectator")
	TSubclassOf<AActor> SpectatorViewpointClass;

	UPROPERTY(EditDefaultsOnly, Category = "BlackHole")
	TSubclassOf<AFPSBlackHole> BlackHoleClass;

};



