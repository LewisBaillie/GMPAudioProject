// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PlotSpherePoints.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GMPAUDIOPROJECT_API UPlotSpherePoints : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlotSpherePoints();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Point Placement Variables
	void PointProductionMaths(int numOfPoints, float GoldenRatio);
	void addPoint(float x, float y, int i);

	FVector2D points[720];
	FVector2D pointToAdd;

	float goldenRatio;
	//End of Placement Variables


	//Ray Cast Variables
	float attenuationDistance;
	FVector startPos;
	FVector endPos;
	FHitResult hit;

	FCollisionQueryParams collisionParameters;

	void RunWave();

};
