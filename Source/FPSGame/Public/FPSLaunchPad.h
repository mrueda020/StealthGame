// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSLaunchPad.generated.h"
class UBoxComponent;

UCLASS()
class FPSGAME_API AFPSLaunchPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSLaunchPad();

protected:
	
	//Mesh component
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UStaticMeshComponent* MeshComp;
	//This triggers the overlap component
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UBoxComponent* OverlapComp;
	// FX effect
	UPROPERTY(EditDefaultsOnly, Category = "FX")
		UParticleSystem* LaunchPadEffect;

	//The total impulse added to the object or actor
	//EditInstanceOnly only allow in-level editing of this property
	UPROPERTY(EditInstanceOnly, Category = "LaunchPad")
		float LauchStrength;

	//Angle added on top of actor rotation to lauch the character
	UPROPERTY(EditInstanceOnly, Category = "LaunchPad")
		float LauchPitchAngle;

	//Function to handle the overlap events is UFUNCTION because we want to bind the overlap events
	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);



};
