// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

USTRUCT()
struct FSpawnPosition
{
	GENERATED_USTRUCT_BODY()

	FVector Location;
	float Rotation = 0.f;
	float Scale = 1.f;
};

USTRUCT(BlueprintType)
struct FRandomSpawnParameters
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn Parameters Struct")
	int32 MinSpawn = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn Parameters Struct")
	int32 MaxSpawn = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn Parameters Struct")
	float MinScale = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn Parameters Struct")
	float MaxScale = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn Parameters Struct")
	float Radius = 500.f;
};

class UActorPool;

UCLASS()
class TESTINGGROUNDS_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void PlaceActors(TSubclassOf<AActor> ToSpawn, FRandomSpawnParameters SpawnParameters);

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void PlaceAIPawns(TSubclassOf<APawn> ToSpawn, FRandomSpawnParameters SpawnParameters);

	UFUNCTION(BlueprintCallable, Category = "Pool")
	void SetPool(UActorPool* PoolToSet);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MinExtent;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MaxExtent;

	UPROPERTY(EditDefaultsOnly, Category = "Navigation")
	FVector NavigationBoundsOffset;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:

	bool CanSpawnAtLocation(FVector Location, float Radius);
	
	bool FindEmptyLocation(FVector& OutLocation, float Radius);

	void PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition SpawnPosition);

	void PlaceAIPawn(TSubclassOf<AActor> ToSpawn, FSpawnPosition SpawnPosition);

	void PositionNavMeshBoundsVolume();

	TArray<FSpawnPosition> GenerateSpawnPositions(FRandomSpawnParameters SpawnParameters);

	UActorPool* Pool;

	AActor* NavMeshBoundsVolume;

	TArray<AActor*> SpawnedActors;
};
