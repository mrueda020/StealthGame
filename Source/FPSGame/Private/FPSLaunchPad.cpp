// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSLaunchPad.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSLaunchPad::AFPSLaunchPad()
{
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));

	OverlapComp->SetBoxExtent(FVector(75,75,50));
	RootComponent = OverlapComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	//Bind to event 
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::HandleOverlap);

	LauchStrength = 1500.0f;
	LauchPitchAngle = 45.0f;


}

void AFPSLaunchPad::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//We obtain the direction
	FRotator LaunchDirection = GetActorRotation();
	//With Pitch we move in the rigth direction (Y-axis)
	LaunchDirection.Pitch += LauchPitchAngle;
	//We convert our direction to an vector
	FVector LauchVelocity = LaunchDirection.Vector() * LauchStrength;

	//We cast out Actor to a Character
	ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
	if (OtherCharacter)
	{	//We Lauch the Character, the booleans give consistant velocity by ignoring the character current velocity
		OtherCharacter->LaunchCharacter(LauchVelocity, true, true);
		//Spawn a fx effect
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LaunchPadEffect, GetActorLocation());


	}
	//If the cast fails it means that is an other component
	else if(OtherComp&&OtherComp->IsSimulatingPhysics())
	{
		//Is the same logic from above but we use AddImpulse method
		OtherComp->AddImpulse(LauchVelocity,NAME_None,true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LaunchPadEffect, GetActorLocation());

	}
}
