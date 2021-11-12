// Fill out your copyright notice in the Description page of Project Settings.


#include "PlotSpherePoints.h"

// Sets default values for this component's properties
UPlotSpherePoints::UPlotSpherePoints()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	int numOfPoints = 720;
	goldenRatio = (1 + sqrt(5)) / 2;
	PointProductionMaths(numOfPoints, goldenRatio);
	// ...
}

void UPlotSpherePoints::PointProductionMaths(int numOfPoints, float GoldenRatio)
{
	for (int i= 0; i < numOfPoints; i++)
	{
		float distance = i / (numOfPoints - 1.0f);
		float angle = 2 * PI * GoldenRatio * i;

		float x = distance * cos(angle);
		float y = distance * sin(angle);

		addPoint(x, y, i);
	}
}

void UPlotSpherePoints::addPoint(float x, float y, int i)
{
	pointToAdd.Set(x, y);
	points[i] = pointToAdd;
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

