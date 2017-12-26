// Fill out your copyright notice in the Description page of Project Settings.

#include "Mannequin.h"
#include "Weapons/Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"

// Sets default values
AMannequin::AMannequin()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Setup Player Camera
	PlayerCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	PlayerCameraComponent->SetupAttachment(GetCapsuleComponent());
	PlayerCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	PlayerCameraComponent->bUsePawnControlRotation = true;

	//Setup FirstPersonMesh
	PlayerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPArms"));
	PlayerMesh->SetOnlyOwnerSee(true);
	PlayerMesh->SetupAttachment(PlayerCameraComponent);
	PlayerMesh->bCastDynamicShadow = false;
	PlayerMesh->CastShadow = false;
	PlayerMesh->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	PlayerMesh->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);
}

// Called when the game starts or when spawned
void AMannequin::BeginPlay()
{
	Super::BeginPlay();
	
	//Attach Gun
	if (GunBlueprint == NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("Gun blueprint missing!"));
		return;
	}
	Gun = GetWorld()->SpawnActor<AGun>(GunBlueprint);
	if (IsPlayerControlled())
	{
		//Attach to first person mesh
		Gun->AttachToComponent(PlayerMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	}
	else
	{
		//Attach to third person mesh
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint_0"));
	}
	Gun->AnimInstanceFP = PlayerMesh->GetAnimInstance();
	Gun->AnimInstanceTP = GetMesh()->GetAnimInstance();
	if (InputComponent != NULL)
	{
		InputComponent->BindAction("Fire", IE_Pressed, this, &AMannequin::PullTrigger);
	}
}

// Called every frame
void AMannequin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMannequin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMannequin::UnPossessed()
{
	Super::UnPossessed();
	if (Gun == NULL)
	{
		return;
	}
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint_0"));
}


void AMannequin::PullTrigger()
{
	Gun->OnFire();
}