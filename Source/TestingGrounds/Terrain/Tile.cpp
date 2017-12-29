// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float MinScale, float MaxScale, float Radius)
{
	int32 NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	float RandomScale = FMath::RandRange(MinScale, MaxScale);
	for (int32 i = 0; i < NumberToSpawn; i++)
	{
		FVector SpawnPoint;
		if (FindEmptyLocation(SpawnPoint, Radius * RandomScale))
		{
			float RandomRotation = FMath::RandRange(-180.0f, 180.0f);
			PlaceActor(ToSpawn, SpawnPoint, RandomRotation , RandomScale);
		}
	}
}

bool ATile::FindEmptyLocation(FVector& OutLocation, float Radius)
{
	const uint32 MAX_ATTEMPTS = 10;
	for (uint32 i = 0; i < MAX_ATTEMPTS; i++)
	{
		FVector RandomPoint = FMath::RandPointInBox(FBox(FVector(0.0f, -2000.0f, 0.0f), FVector(4000.0f, 2000.0f, 0.0f)));
		if (CanSpawnAtLocation(RandomPoint, Radius))
		{
			OutLocation = RandomPoint;
			return true;
		}
	}
	return false;
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FVector SpawnPoint, float Rotation, float Scale)
{
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ToSpawn);
	SpawnedActor->SetActorRelativeLocation(SpawnPoint);
	SpawnedActor->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	SpawnedActor->SetActorRotation(FRotator(0, Rotation, 0));
	SpawnedActor->SetActorRelativeScale3D(FVector(Scale));
}

bool ATile::CanSpawnAtLocation(FVector Location, float Radius)
{
	FHitResult HitResult;
	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);
	bool HasHit = GetWorld()->SweepSingleByChannel(HitResult, GlobalLocation, GlobalLocation, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeSphere(Radius));
	return !HasHit;
}
