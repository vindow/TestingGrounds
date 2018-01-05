// Fill out your copyright notice in the Description page of Project Settings.

#include "InfiniteTerrainGameMode.h"
#include "AI/Navigation/NavMeshBoundsVolume.h"
#include "EngineUtils.h"
#include "ActorPool.h"
#include "UObject/ConstructorHelpers.h"
#include "Terrain/Tile.h"
#include "GameFramework/HUD.h"


AInfiniteTerrainGameMode::AInfiniteTerrainGameMode()
{
	NavMeshBoundsVolumePool = CreateDefaultSubobject<UActorPool>(FName("Nav Mesh Bounds Volume Pool"));

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerPawnClassFinder(TEXT("/Game/Dynamic/Character/Behaviour/BP_PlayerController"));
	PlayerControllerClass = PlayerPawnClassFinder.Class;
	ReplaySpectatorPlayerControllerClass = PlayerPawnClassFinder.Class;

	static ConstructorHelpers::FClassFinder<APawn> PawnClassFinder(TEXT("/Game/Dynamic/Character/Behaviour/BP_Character"));
	DefaultPawnClass = PawnClassFinder.Class;

	static ConstructorHelpers::FClassFinder<AHUD> HUDClassFinder(TEXT("/Game/Dynamic/UI/BP_HUD"));
	HUDClass = HUDClassFinder.Class;

	static ConstructorHelpers::FObjectFinder<UBlueprint> TileBlueprintFinder(TEXT("Blueprint'/Game/Dynamic/Terrain/BP_Tile.BP_Tile'"));
	if (TileBlueprintFinder.Object) {
		TileBlueprint = (UClass*)TileBlueprintFinder.Object->GeneratedClass;
	}

	TileTransform = FTransform();
}

void AInfiniteTerrainGameMode::BeginPlay()
{
	Super::BeginPlay();
	PopulateBoundsPool();
	SpawnTile();
}

void AInfiniteTerrainGameMode::PopulateBoundsPool()
{
	TActorIterator<ANavMeshBoundsVolume> NavMeshIterator = TActorIterator<ANavMeshBoundsVolume>(GetWorld());
	while (NavMeshIterator)
	{
		AddToPool(*NavMeshIterator);
		++NavMeshIterator;
	}
}

int32 AInfiniteTerrainGameMode::GetScore()
{
	return Score;
}

void AInfiniteTerrainGameMode::NewTileConquered()
{
	Score++;
}

void AInfiniteTerrainGameMode::AddToPool(ANavMeshBoundsVolume * VolumeToAdd)
{
	NavMeshBoundsVolumePool->Add(VolumeToAdd);
}

void AInfiniteTerrainGameMode::SpawnTile()
{
	UWorld* const World = GetWorld();
	if (World) {
		ATile* Tile = World->SpawnActor<ATile>(TileBlueprint, TileTransform);
		TileTransform.AddToTranslation(FVector(4000, 0, 0));
		if (Tile)
		{
			Tile->SetPool(NavMeshBoundsVolumePool);
		}
	}
}
