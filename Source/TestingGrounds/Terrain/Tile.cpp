// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "ActorPool.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Character/Mannequin.h"

// Sets default values
ATile::ATile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MinExtent = FVector(0.f, -2000.f, 0.f);
	MaxExtent = FVector(4000.f, 2000.f, 0.f);
	NavigationBoundsOffset = FVector(2025.f, 0.f, 0.f);
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (Pool != nullptr && NavMeshBoundsVolume != nullptr)
	{
		Pool->Return(NavMeshBoundsVolume);
	}
	for (int32 i = 0; i < SpawnedActors.Num(); i++)
	{
		SpawnedActors[i]->Destroy();
	}
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATile::SetPool(UActorPool* PoolToSet)
{
	Pool = PoolToSet;
	PositionNavMeshBoundsVolume();
}

void ATile::PositionNavMeshBoundsVolume()
{
	NavMeshBoundsVolume = Pool->Checkout();
	if (NavMeshBoundsVolume == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Not enough actors in pool!"), *GetName());
		return;
	}
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + NavigationBoundsOffset);
	GetWorld()->GetNavigationSystem()->Build();
}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, FRandomSpawnParameters SpawnParameters)
{
	RandomlyPlaceActors(ToSpawn, SpawnParameters);
}

void ATile::PlaceAIPawns(TSubclassOf<APawn> ToSpawn, FRandomSpawnParameters SpawnParameters)
{
	RandomlyPlaceActors(ToSpawn, SpawnParameters);
}

template <class T>
void ATile::RandomlyPlaceActors(TSubclassOf<T> ToSpawn, FRandomSpawnParameters SpawnParameters)
{
	int32 NumberToSpawn = FMath::RandRange(SpawnParameters.MinSpawn, SpawnParameters.MaxSpawn);
	for (int32 i = 0; i < NumberToSpawn; i++)
	{
		FSpawnPosition SpawnPosition;
		SpawnPosition.Scale = FMath::RandRange(SpawnParameters.MinScale, SpawnParameters.MaxScale);
		if (FindEmptyLocation(SpawnPosition.Location, SpawnParameters.Radius * SpawnPosition.Scale))
		{
			SpawnPosition.Rotation = FMath::RandRange(-180.0f, 180.0f);
			PlaceActor(ToSpawn, SpawnPosition);
		}
	}
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition SpawnPosition)
{
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ToSpawn);
	if (SpawnedActor)
	{
		SpawnedActor->SetActorRelativeLocation(SpawnPosition.Location);
		SpawnedActor->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		SpawnedActor->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
		SpawnedActor->SetActorRelativeScale3D(FVector(SpawnPosition.Scale));
		SpawnedActors.Add(SpawnedActor);
	}
}

void ATile::PlaceActor(TSubclassOf<APawn> ToSpawn, FSpawnPosition SpawnPosition)
{
	FRotator Rotation = FRotator(0, SpawnPosition.Rotation, 0);
	APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(ToSpawn, SpawnPosition.Location, Rotation);
	if (SpawnedPawn)
	{
		SpawnedPawn->Tags.Add(FName("Enemy"));
		SpawnedPawn->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		SpawnedPawn->SpawnDefaultController();
		Cast<AMannequin>(SpawnedPawn)->SetPatrolledTile(this);
		SpawnedPawns.Add(SpawnedPawn);
	}
}

bool ATile::FindEmptyLocation(FVector& OutLocation, float Radius)
{
	const uint32 MAX_ATTEMPTS = 10;
	for (uint32 i = 0; i < MAX_ATTEMPTS; i++)
	{
		FVector RandomPoint = FMath::RandPointInBox(FBox(MinExtent, MaxExtent));
		if (CanSpawnAtLocation(RandomPoint, Radius))
		{
			OutLocation = RandomPoint;
			return true;
		}
	}
	return false;
}

bool ATile::CanSpawnAtLocation(FVector Location, float Radius)
{
	FHitResult HitResult;
	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);
	bool HasHit = GetWorld()->SweepSingleByChannel(HitResult, GlobalLocation, GlobalLocation, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeSphere(Radius));
	return !HasHit;
}
