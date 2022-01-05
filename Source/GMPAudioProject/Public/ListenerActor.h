// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ListenerActor.generated.h"

UCLASS()
class GMPAUDIOPROJECT_API AListenerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AListenerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void TakeInformation(TArray<FVector> deadPoints, TArray<FVector> hitPoints, TArray<FVector> hitAngles, TArray<float> hitDistances);
	
	UPROPERTY()
	TArray<FVector> DeadPoints;

	UPROPERTY()
	TArray<FVector> HitPoints;

	UPROPERTY()
	TArray<FVector> HitAngles;

	UPROPERTY()
	TArray<float> HitDistances;

	FVector preCalcAngle;
	FVector preCalcDeadDistance;

	float averageDistance;
	float averageRetainedDistance;
	FVector averageAngle;
	FVector assumedLocation;

	float Volume;
	float VolumeSixty;
	float VolumeFourty;

	void calculateEffects();


};
