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

