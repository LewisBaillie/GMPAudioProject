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

	//Input Method
	/*virtual void SetupInputComponent(UInputComponent* PlayerInputComponent);*/

	//Input Variable
	/*UInputComponent* inputComponent;*/


	//Point Placement Variables
	UFUNCTION(BlueprintCallable)
	void PointProductionMaths(int NumOfPoints, float GoldenRatio);
	void addPoint(float x, float y, float z, int i);

	FVector points[720];
	FVector pointToAdd;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	int numOfPoints;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	float goldenRatio;
	//End of Placement Variables


	//Ray Cast Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
	float attenuationDistance = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
	FVector soundLocation;

	FVector startPos;
	FVector endPos;
	FVector directionalToPoint;
	int vectorLength;
	FVector normalizedVector;

	bool rayHit;

	FCollisionQueryParams collisionParameters;

	void RunWave();

};
