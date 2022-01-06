// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ListenerActor.h"
#include "Sound/SoundCue.h"
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

	UFUNCTION(BlueprintCallable)
	void setCue(USoundCue* Cue);

	UFUNCTION(BlueprintCallable)
	void setListener(AListenerActor* Listener);
	
	UPROPERTY()
	TArray<FVector> points;
	UPROPERTY()
	TArray<FVector> pointsSecondary;
	UPROPERTY()
	TArray<float> distances;
	UPROPERTY()
	TArray<float> distancesSecondary;

	int hitsFound;

	FVector reflectionAngle;
	UPROPERTY()
	TArray<FVector> reflectionAngles;
	UPROPERTY()
	TArray<FVector> reflectionAnglesSecondary;

	UPROPERTY()
	TArray<FVector> endPoints;
	UPROPERTY()
	TArray<FVector> hitPoints;
	UPROPERTY()
	TArray<float> hitDistances;
	UPROPERTY()
	TArray<FVector> hitAngles;


	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	int numOfPoints;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	float goldenRatio;
	//End of Placement Variables

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USoundCue* cue;

	//Ray Cast Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
	float attenuationDistance = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
	FVector soundLocation;

	FVector startPos;
	FVector endPos;
	FVector directionalToPoint;
	int vectorLength;
	FVector normalizedVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AListenerActor* listener;
	
	bool rayHit;

	int active;
	int waveCount;

	FCollisionQueryParams collisionParameters;

	void RunFirstWave();
	void RunWave(TArray<FVector> startPositions, TArray<float> distance, TArray<FVector> reflectionAngleArray);
	void RunNextWave(TArray<FVector> positions, TArray<float> distancesList, TArray<FVector> reflectionAngleList);

	void EndOfWaves();


};
 