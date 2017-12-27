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

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn)
{
	int32 NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	for (int32 i = 0; i < NumberToSpawn; i++)
	{
		auto SpawnPoint = FMath::RandPointInBox(FBox(FVector(0.0f, -2000.0f, 0.0f), FVector(4000.0f, 2000.0f, 0.0f)));
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ToSpawn);
		SpawnedActor->SetActorRelativeLocation(SpawnPoint);
		SpawnedActor->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	}
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	CastSphere(GetActorLocation(), 300.0f);
	CastSphere(GetActorLocation() + FVector(0, 0, 500), 300.0f);
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ATile::CastSphere(FVector Location, float Radius)
{
	FHitResult HitResult;
	bool HasHit = GetWorld()->SweepSingleByChannel(HitResult, Location, Location, FQuat::Identity, ECollisionChannel::ECC_Camera, FCollisionShape::MakeSphere(Radius));
	if (HasHit)
	{
		DrawDebugSphere(GetWorld(), Location, Radius, 32, FColor(255, 0, 0), true, -1.0f, (uint8)'\000', 1.0f);
	}
	else
	{
		DrawDebugSphere(GetWorld(), Location, Radius, 32, FColor(0, 255, 0), true, -1.0f, (uint8)'\000', 1.0f);
	}
	return HasHit;
}