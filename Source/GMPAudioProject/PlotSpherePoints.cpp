// Fill out your copyright notice in the Description page of Project Settings.


#include "PlotSpherePoints.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UPlotSpherePoints::UPlotSpherePoints()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	numOfPoints = 720;
	goldenRatio =  (1+ sqrt(5))/2;
	// ...
}

//PointProductionMaths(numOfPoints, goldenRatio);

//void UPlotSpherePoints::SetupInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupInputComponent(PlayerInputComponent);
//	
//}


void UPlotSpherePoints::PointProductionMaths(int NumOfPoints, float GoldenRatio)
{
	for (int i= 0; i < numOfPoints; i++)
	{
		float angle = 2 * PI * i/GoldenRatio;
		float phi = acos(1 - 2 * (i + 0.5) / NumOfPoints);


		float x = cos(angle) * sin(phi);
		float y = sin(angle) * sin(phi);
		float z = cos(phi);

		

		y = y + GetOwner()->GetActorLocation().Y;
		x = x + GetOwner()->GetActorLocation().X;
		z = z + GetOwner()->GetActorLocation().Z;
		


		addPoint(x, y, z, i);

	}
	RunWave();
}

void UPlotSpherePoints::addPoint(float x, float y, float z, int i)
{
	pointToAdd.Set(x, y, z);
	points[i] = pointToAdd;
}


void UPlotSpherePoints::RunWave()
{
	for (int i = 0; i < sizeof(points); i++)
	{
		startPos = points[i];
		directionalToPoint = points[i] - GetOwner()->GetActorLocation();
		vectorLength = ((directionalToPoint.X * directionalToPoint.X) + (directionalToPoint.Y * directionalToPoint.Y) + (directionalToPoint.Z * directionalToPoint.Z));
		normalizedVector = points[i] / vectorLength;

		endPos = startPos + (normalizedVector * attenuationDistance);
		FHitResult hit;

		rayHit = GetOwner()->ActorLineTraceSingle(hit, startPos, endPos, ECollisionChannel::ECC_WorldDynamic, collisionParameters);

		DrawDebugLine(GetWorld(), startPos, endPos, FColor::Green, true, 5, 0, 2.f);
	}
}

// Called when the game starts
void UPlotSpherePoints::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPlotSpherePoints::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

