// Fill out your copyright notice in the Description page of Project Settings.

#include "GrassComponent.h"

UGrassComponent::UGrassComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGrassComponent::BeginPlay()
{
	Super::BeginPlay();
	SpawnGrass();
}


void UGrassComponent::SpawnGrass()
{
	for (int32 i = 0; i < SpawnCount; i++)
	{
		FVector RandomPoint = FMath::RandPointInBox(SpawningExtents);
		float RandomRotation = FMath::FRandRange(-180.0f, 180.0f);
		AddInstance(FTransform(FRotator(0, RandomRotation, 0), RandomPoint));
	}
}
