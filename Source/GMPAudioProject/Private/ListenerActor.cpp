// Fill out your copyright notice in the Description page of Project Settings.


#include "ListenerActor.h"


// Sets default values
AListenerActor::AListenerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AListenerActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AListenerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AListenerActor::TakeInformation(TArray<FVector> deadPoints, TArray<FVector> hitPoints, TArray<FVector> hitAngles, TArray<float> hitDistances)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("INFO TAKEN")));
	DeadPoints = deadPoints;
	HitPoints = hitPoints;
	HitAngles = hitAngles;
	HitDistances = hitDistances;
	calculateEffects();
}

void AListenerActor::calculateEffects()
{
	for (FVector i : DeadPoints)
	{
		preCalcDeadDistance += (GetOwner()->GetActorLocation() - i);
		averageAngle += (GetOwner()->GetActorLocation() - i);
	}
	averageDistance = (sqrt((preCalcDeadDistance.X * preCalcDeadDistance.X) + (preCalcDeadDistance.Y * preCalcDeadDistance.Y) + (preCalcDeadDistance.Z * preCalcDeadDistance.Z)))/DeadPoints.Num();

	for (float i : HitDistances)
	{
		averageRetainedDistance += (1000 - i);
	}
	averageRetainedDistance = averageRetainedDistance / HitDistances.Num();

	for (FVector i : HitAngles)
	{
		averageAngle += i;
	}

	averageAngle = averageAngle / sqrt((averageAngle.X * averageAngle.X) + (averageAngle.Y * averageAngle.Y) + (averageAngle.Z * averageAngle.Z));
	assumedLocation = averageAngle * averageDistance;

	if (averageRetainedDistance > 100)
	{
		VolumeSixty = 60;
	}
	else if (averageRetainedDistance > 75 && averageRetainedDistance < 100)
	{
		VolumeSixty = 45;
	}
	else if (averageRetainedDistance > 50 && averageRetainedDistance < 75)
	{
		VolumeSixty = 30;
	}
	else if (averageRetainedDistance > 25 && averageRetainedDistance < 50)
	{
		VolumeSixty = 15;
	}
	else if (averageRetainedDistance > 0 && averageRetainedDistance < 25)
	{
		VolumeSixty = 0;
	}

	if (averageDistance <= 50)
	{
		VolumeFourty = 40;
	}
	else if (averageDistance <= 125)
	{
		VolumeFourty = 30;
	}
	else if (averageDistance <= 200)
	{
		VolumeFourty = 20;
	}
	else if (averageDistance <= 275)
	{
		VolumeFourty = 10;
	}
	else if (averageDistance > 275)
	{
		VolumeFourty = 0;
	}

	Volume = (VolumeSixty) + (VolumeFourty);


	

}
