// Fill out your copyright notice in the Description page of Project Settings.


#include "PlotSpherePoints.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"

// Sets default values for this component's properties
UPlotSpherePoints::UPlotSpherePoints()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	numOfPoints = 100;
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

	points.Empty();
	
	float phi = PI * (3.f - sqrt(5.f));
	//static_cast<float>
	for (int i= 0; i < NumOfPoints; i++)
	{
		float y = (i / (float)(NumOfPoints - 1));
		y = y * 2;
		y = 1 - y;
		

		float radius = sqrt(1-y*y);

		float theta = phi * i;

		float x = cos(theta) * radius;
		float z = sin(theta) * radius;

		y = y + GetOwner()->GetActorLocation().Y;
		x = x + GetOwner()->GetActorLocation().X;
		z = z + GetOwner()->GetActorLocation().Z;

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%f, %f, %f"), x, y, z));

		points.Emplace(x, y, z);

		/*float angle = 2 * PI * i/GoldenRatio;
		float phi = acos(1 - 2 * (i + 0.5) / NumOfPoints);


		float x = cos(angle) * sin(phi);
		float y = sin(angle) * sin(phi);
		float z = cos(phi);

		
		

		
		


		*/

	}
	RunFirstWave();
}




void UPlotSpherePoints::RunFirstWave()
{
	/*for (int i = 0; i < sizeof(points)/sizeof(points[0]); i++)
	{
		
	}*/
	active = 1;
	for (FVector point : points)
	{
		FHitResult hit;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%f, %f, %f"), point.X, point.Y, point.Z));
		startPos = point;
		directionalToPoint = point - GetOwner()->GetActorLocation();
		vectorLength = sqrt((directionalToPoint.X * directionalToPoint.X) + (directionalToPoint.Y * directionalToPoint.Y) + (directionalToPoint.Z * directionalToPoint.Z));
		normalizedVector = (point - GetOwner()->GetActorLocation()) / vectorLength;

		endPos = startPos + (normalizedVector * attenuationDistance);
		
		rayHit = GetWorld()->LineTraceSingleByChannel(hit, startPos, endPos, ECollisionChannel::ECC_MAX);

		if (rayHit)
		{
			/*The hit is repeatig due to the pixel perfect collisio with the sae aterial*/
			pointsSecondary.Emplace(hit.ImpactPoint + (normalizedVector * -0.1f));
			distancesSecondary.Emplace(attenuationDistance - hit.Distance);
			
			reflectionAngle = normalizedVector - (2 * (FVector::DotProduct(normalizedVector, hit.ImpactNormal)) / (hit.ImpactNormal.X * hit.ImpactNormal.X) + (hit.ImpactNormal.Y * hit.ImpactNormal.Y) + (hit.ImpactNormal.Z * hit.ImpactNormal.Z)) * hit.ImpactNormal;
			reflectionAnglesSecondary.Emplace(reflectionAngle);

			
			hit.Reset(0, false);
		}
		DrawDebugLine(GetWorld(), startPos, endPos, FColor::Green, true, 5, 0, 2.f);
		rayHit = false;
	}
	if (pointsSecondary.Num() > 0)
	{
		
		points.Empty();
		RunNextWave(pointsSecondary, distancesSecondary, reflectionAnglesSecondary);
	}
}

void UPlotSpherePoints::RunWave(TArray<FVector> startPositions, TArray<float> distance, TArray<FVector> reflectionAngleArray)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("STARTING WAVE")));

	if (active == 2)
	{
		active = 1;
	}
	else if (active == 1)
	{
		active = 2;
	}


	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%d"),startPositions.Num()));
	for (int i = 0; i < startPositions.Num(); i++)
	{
		FHitResult hitOut;

		//startPos = point;
		/*directionalToPoint = point - GetOwner()->GetActorLocation();
		vectorLength = sqrt((directionalToPoint.X * directionalToPoint.X) + (directionalToPoint.Y * directionalToPoint.Y) + (directionalToPoint.Z * directionalToPoint.Z));
		normalizedVector = (point - GetOwner()->GetActorLocation()) / vectorLength;

		endPos = startPos + (normalizedVector * attenuationDistance);

		rayHit = GetWorld()->LineTraceSingleByChannel(hit, startPos, endPos, ECollisionChannel::ECC_WorldStatic, collisionParameters);*/


		rayHit = GetWorld()->LineTraceSingleByChannel(hitOut, startPositions[i], startPositions[i] + (reflectionAngleArray[i] * distance[i]), ECollisionChannel::ECC_WorldStatic, collisionParameters);

		if (rayHit)
		{
			hitsFound += 1;
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("HIT")));
			if (active == 2)
			{
				points.Emplace(hitOut.ImpactPoint + (reflectionAngleArray[i] * -0.1f));
				distances.Emplace(distance[i] - hitOut.Distance);

				reflectionAngle = reflectionAngleArray[i] - (2 * (FVector::DotProduct(reflectionAngleArray[i], hitOut.ImpactNormal)) / (hitOut.ImpactNormal.X * hitOut.ImpactNormal.X) + (hitOut.ImpactNormal.Y * hitOut.ImpactNormal.Y) + (hitOut.ImpactNormal.Z * hitOut.ImpactNormal.Z)) * hitOut.ImpactNormal;
				/*vectorLength = sqrt((reflectionAngle.X * reflectionAngle.X) + (reflectionAngle.Y * reflectionAngle.Y) + (reflectionAngle.Z * reflectionAngle.Z));
				reflectionAngle = (reflectionAngle / vectorLength);*/
				reflectionAngles.Emplace(reflectionAngle);
			}
			else if (active == 1)
			{
				pointsSecondary.Emplace(hitOut.ImpactPoint + (reflectionAngleArray[i] * -0.1f));
				distancesSecondary.Emplace(distance[i] - hitOut.Distance);

				reflectionAngle = reflectionAngleArray[i] - (2 * (FVector::DotProduct(reflectionAngleArray[i], hitOut.ImpactNormal)) / (hitOut.ImpactNormal.X * hitOut.ImpactNormal.X) + (hitOut.ImpactNormal.Y * hitOut.ImpactNormal.Y) + (hitOut.ImpactNormal.Z * hitOut.ImpactNormal.Z)) * hitOut.ImpactNormal;
				/*vectorLength = sqrt((reflectionAngle.X * reflectionAngle.X) + (reflectionAngle.Y * reflectionAngle.Y) + (reflectionAngle.Z * reflectionAngle.Z));
				reflectionAngle = (reflectionAngle / vectorLength);*/
				reflectionAnglesSecondary.Emplace(reflectionAngle);
			}
			
		}

		DrawDebugLine(GetWorld(), startPositions[i], startPositions[i] + (reflectionAngleArray[i] * distance[i]), FColor::Green, true, 5, 0, 2.f);

		hitOut.Reset(0, false);
		rayHit = false;
	}

	if (pointsSecondary.Num() > 0 && active == 1)
	{	
		hitsFound = 0;
		if (points.IsValidIndex(0))
		{
			points.Empty();
		}
		if (distances.IsValidIndex(0))
		{
			distances.Empty();
		}
		if (reflectionAngles.IsValidIndex(0))
		{
			reflectionAngles.Empty();
		}
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%f, %f, %f"), points[0].X, points[0].Y, points[0].Z));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%f, %f, %f"), reflectionAnglesSecondary[0].X, reflectionAnglesSecondary[0].Y, reflectionAnglesSecondary[0].Z));
		RunNextWave(pointsSecondary, distancesSecondary, reflectionAnglesSecondary);
	}
	if (points.Num() > 0 && active == 2)
	{
		hitsFound = 0;
		if (pointsSecondary.IsValidIndex(0))
		{
			pointsSecondary.Empty();
		}
		if (distancesSecondary.IsValidIndex(0))
		{
			distancesSecondary.Empty();
		}
		if (reflectionAnglesSecondary.IsValidIndex(0))
		{
			reflectionAnglesSecondary.Empty();
		}
		RunNextWave(points, distances, reflectionAngles);
	}

}


void UPlotSpherePoints::RunNextWave(TArray<FVector> positions, TArray<float> distancesList, TArray<FVector> reflectionAngleList)
{
	RunWave(positions, distancesList, reflectionAngleList);
}

// Called when the game starts
void UPlotSpherePoints::BeginPlay()
{
	Super::BeginPlay();
	// ...
	/*distances.Init(1000, 1);
	reflectionAngles.Init(FVector(0, 0, 0), 1);
	points.Init(FVector(0, 0, 0), 1);
	distancesSecondary.Init(1000, 1);
	reflectionAnglesSecondary.Init(FVector(0, 0, 0), 1);
	pointsSecondary.Init(FVector(0, 0, 0), 1);*/
}


// Called every frame
void UPlotSpherePoints::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

