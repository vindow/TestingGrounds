// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestingGroundsGameMode.h"
#include "InfiniteTerrainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TESTINGGROUNDS_API AInfiniteTerrainGameMode : public ATestingGroundsGameMode
{
	GENERATED_BODY()
	
public:
	AInfiniteTerrainGameMode();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Bounds Pool")
	void PopulateBoundsPool();

	UFUNCTION(BlueprintCallable, Category = "Score")
	int32 GetScore();

	UFUNCTION(BlueprintCallable, Category = "Score")
	void NewTileConquered();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bounds Pool")
	class UActorPool* NavMeshBoundsVolumePool;

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnTile();
private:
	void AddToPool(class ANavMeshBoundsVolume * VolumeToAdd);

	

	FTransform TileTransform;

	int32 Score = 0;

	TSubclassOf<class ATile> TileBlueprint;
};
