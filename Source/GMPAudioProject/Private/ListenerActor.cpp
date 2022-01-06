// Fill out your copyright notice in the Description page of Project Settings.


#include "ListenerActor.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
AListenerActor::AListenerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));

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

void AListenerActor::TakeInformation(TArray<FVector> deadPoints, TArray<FVector> hitPoints, TArray<FVector> hitAngles, TArray<float> hitDistances, USoundCue* cue)
{
	

	
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("INFO TAKEN")));
	DeadPoints = deadPoints;
	HitPoints = hitPoints;
	HitAngles = hitAngles;
	HitDistances = hitDistances;
	Cue = cue;
	calculateEffects();
}

void AListenerActor::calculateEffects()
{
	preCalcDeadDistance = 0;
	preCalcAngle = FVector(0.f,0.f,0.f);
	averageDistance = 0;
	averageRetainedDistance = 0;
	averageAngle = FVector(0.f,0.f,0.f);
	assumedLocation = FVector(0.f,0.f,0.f);

	FVector tempAdd = FVector(0,0,0);
	
	FVector actorPos = GetActorLocation();
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("ACTOR LOCATION %f,%f,%f"),GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%f,%f,%f"),tempAdd.X, tempAdd.Y, tempAdd.Z));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%f"),tempAdd.X));
	float num = 0;

	int intHolder = 0;
	float sizeTemp = 0;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%f"),holder));
	 for (int i = 0; i < DeadPoints.Num(); i++)
	 {
	 	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%d"),intHolder));
	 	FVector temp = actorPos - DeadPoints[i];
	 	sizeTemp = temp.Size();

	 	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("BEFORE %f"),sizeTemp));
	 	int intTemp = UE4::SSE::FloorToInt(sizeTemp);
	 	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("AFTER %d"),intTemp));
	 	
	 	//float one = 2000;
	 	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%f"),sizeTemp));
		intHolder += intTemp;
	 	//GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Green,FString::Printf(TEXT("Running")));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%f"),tempAdd.X));
	 	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%f,%f,%f"),tempAdd.X, tempAdd.Y, tempAdd.Z));
	 	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%f,%f,%f"),i.X, i.Y, i.Z));
	 	averageAngle += (actorPos - DeadPoints[i]);
	 	num += 1.0f;
	 }
	
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%d"),num));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%f,%f,%f"),preCalcDeadDistance.X, preCalcDeadDistance.Y, preCalcDeadDistance.Z));
	//preCalcDeadDistance = preCalcDeadDistance/num;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%f,%f,%f"),preCalcDeadDistance.X, preCalcDeadDistance.Y, preCalcDeadDistance.Z));

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("BEFORE %d"),intHolder));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%f"),num));
	intHolder = sqrt(intHolder*intHolder);

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("AFTER %d"),intHolder));
	
	averageDistance = intHolder/(num);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%f"),averageDistance));
	
	for (float i : HitDistances)
	{
		averageRetainedDistance += (1000.0f - i);
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

	if (averageDistance <= 300)
	{
		VolumeFourty = 40;
	}
	else if (averageDistance <= 400)
	{
		VolumeFourty = 30;
	}
	else if (averageDistance <= 500)
	{
		VolumeFourty = 20;
	}
	else if (averageDistance <= 600)
	{
		VolumeFourty = 10;
	}
	else if (averageDistance > 600)
	{
		VolumeFourty = 0;
	}

	Volume = ((VolumeSixty) + (VolumeFourty))/100;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%f"),Volume));

	UGameplayStatics::PlaySound2D(GetWorld(), Cue, Volume);

}
