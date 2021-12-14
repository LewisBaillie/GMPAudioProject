// Fill out your copyright notice in the Description page of Project Settings.


#include "PlotSpherePoints.h"
#include "DrawDebugHelpers.h"

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
	for (FVector point : points)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%f, %f, %f"), point.X, point.Y, point.Z));
		startPos = point;
		directionalToPoint = point - GetOwner()->GetActorLocation();
		vectorLength = sqrt((directionalToPoint.X * directionalToPoint.X) + (directionalToPoint.Y * directionalToPoint.Y) + (directionalToPoint.Z * directionalToPoint.Z));
		normalizedVector = (point - GetOwner()->GetActorLocation()) / vectorLength;

		endPos = startPos + (normalizedVector * attenuationDistance);
		
		rayHit = GetWorld()->LineTraceSingleByChannel(hit, startPos, endPos, ECollisionChannel::ECC_WorldStatic, collisionParameters);

		if (rayHit)
		{
			
			pointsSecondary.Emplace(hit.ImpactPoint);
			distancesSecondary.Emplace(attenuationDistance - hit.Distance);
			
			reflectionAngle = normalizedVector - (2 * (FVector::DotProduct(normalizedVector, hit.ImpactNormal))/ sqrt(hit.ImpactNormal.X * hit.ImpactNormal.X + hit.ImpactNormal.Y * hit.ImpactNormal.Y + hit.ImpactNormal.Z * hit.ImpactNormal.Z)) * hit.ImpactNormal;
			reflectionAnglesSecondary.Emplace(reflectionAngle);
		}

		DrawDebugLine(GetWorld(), startPos, endPos, FColor::Green, true, 5, 0, 2.f);
		hit.Reset();
	}
	if (pointsSecondary.Num() > 0)
	{
		if (points.IsValidIndex(0))
		{
			points.Empty();
		}
		active = 2;
		RunWave(pointsSecondary, distancesSecondary, reflectionAnglesSecondary);
	}
}

void UPlotSpherePoints::RunWave(TArray<FVector> startPositions, TArray<float> distance, TArray<FVector> reflectionAngleArray)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("STARTING WAVE")));

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%d"),startPositions.Num()));
	for (int i = 0; i < startPositions.Num(); i++)
	{
		startPos = startPositions[i];
		endPos = startPos + (reflectionAngleArray[i] * distance[i]);

		rayHit = GetWorld()->LineTraceSingleByChannel(hit, startPos, endPos, ECollisionChannel::ECC_WorldStatic, collisionParameters);

		if (rayHit)
		{
			hitsFound += 1;
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("HIT")));
			if (active == 2)
			{
				
				points.Emplace(hit.ImpactPoint);
				distances.Emplace(distance[i] - hit.Distance);

				reflectionAngle = reflectionAngleArray[i] - (2 * (FVector::DotProduct(reflectionAngleArray[i] , hit.ImpactNormal)) / sqrt(hit.ImpactNormal.X * hit.ImpactNormal.X + hit.ImpactNormal.Y * hit.ImpactNormal.Y + hit.ImpactNormal.Z * hit.ImpactNormal.Z)) * hit.ImpactNormal;
				reflectionAngles.Emplace(reflectionAngle);
			}
			else if (active == 1)
			{
				pointsSecondary.Emplace(hit.ImpactPoint);
				distancesSecondary.Emplace(distance[i] - hit.Distance);

				reflectionAngle = reflectionAngleArray[i] - (2 * (FVector::DotProduct(reflectionAngleArray[i], hit.ImpactNormal)) / sqrt(hit.ImpactNormal.X * hit.ImpactNormal.X + hit.ImpactNormal.Y * hit.ImpactNormal.Y + hit.ImpactNormal.Z * hit.ImpactNormal.Z)) * hit.ImpactNormal;
				reflectionAnglesSecondary.Emplace(reflectionAngle);
			}
		}

		DrawDebugLine(GetWorld(), startPos, endPos, FColor::Green, true, 5, 0, 2.f);

		hit.Reset();
		rayHit = false;
	}

	if (hitsFound > 0 && active == 1)
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
		active = 2;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%f, %f, %f"), reflectionAnglesSecondary[0].X, reflectionAnglesSecondary[0].Y, reflectionAnglesSecondary[0].Z));
		RunWave(pointsSecondary, distancesSecondary, reflectionAnglesSecondary);
	}
	if (hitsFound > 0 && active == 2)
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
		active = 1;
		RunWave(points, distances, reflectionAngles);
	}

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

