// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"

class UPawnSensingComponent;

UENUM(BlueprintType)
enum class EGuardState : uint8 
{
	Idle,
	Suspicious,
	Alerted
};




UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);


	UFUNCTION()
	void OnNoiseHeard(APawn* HeardInstigator, const FVector& Location, float Volume);

	FRotator OriginalRotation;

	FTimerHandle TimerHandle_ResetOrientation;
	//specifies a callback function which is executed when the property is updated over the network.
	UPROPERTY(ReplicatedUsing = OnRep_GuardStateChange)
	EGuardState GuardState;
	// callback function
	UFUNCTION()
	void OnRep_GuardStateChange();

	void SetGuardState(EGuardState NewState);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnGuardStateChange(EGuardState NewState);

	void ResetOrientation();


	UPROPERTY(EditInstanceOnly, Category = "AI")
	bool bPatrol;

	UPROPERTY(EditInstanceOnly, Category = "AI", meta =(EditCondition = "bPatrol" ))
	TArray<AActor*> TargetPoints;

	

	AActor* CurrentTargetPoint;

	void MoveToNextPatrolPoint();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//Returns the properties used for network replication
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
