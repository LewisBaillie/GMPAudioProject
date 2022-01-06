// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sound/SoundCue.h"
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

	void TakeInformation(TArray<FVector> deadPoints, TArray<FVector> hitPoints, TArray<FVector> hitAngles, TArray<float> hitDistances, USoundCue* cue);
	
	UPROPERTY()
	TArray<FVector> DeadPoints;

	UPROPERTY()
	TArray<FVector> HitPoints;

	UPROPERTY()
	TArray<FVector> HitAngles;

	UPROPERTY()
	TArray<float> HitDistances;

	UPROPERTY()
	FVector preCalcAngle;
	
	float preCalcDeadDistance;

	float averageDistance;
	float averageRetainedDistance;
	UPROPERTY()
	FVector averageAngle;
	UPROPERTY()
	FVector assumedLocation;

	float Volume;
	float VolumeSixty;
	float VolumeFourty;

	void calculateEffects();

	USoundCue* Cue;


};
