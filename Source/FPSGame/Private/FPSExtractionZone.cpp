// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSExtractionZone.h"
#include "Components/BoxComponent.h"
#include "components/DecalComponent.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"

// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	// Enum indicating how each type should respond
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	//We only want care about the pawn
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	//We created a vector of 200 unit size in x,y,z
	OverlapComp->SetBoxExtent(FVector(200.f));

	//The extraction zone is visible in game running
	OverlapComp->SetHiddenInGame(false);

	RootComponent = OverlapComp;
	
	//We call our method to handle overlapevents
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::HandleOverlap);

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->DecalSize = FVector(200.f);
	DecalComp->SetupAttachment(RootComponent);
}


void AFPSExtractionZone::HandleOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	AFPSCharacter * Pawn = Cast<AFPSCharacter>(OtherActor);

	if (Pawn&&Pawn->bIsCarryingObjective)
	{
		AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
		if (GM) 
		{	
			UE_LOG(LogTemp, Log, TEXT("Mission Completed!"));
			GM->CompleteMission(Pawn);
		}

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Find the object first!"));
	}
}



