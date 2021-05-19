// Fill out your copyright notice in the Description page of Project Settings.
#include "Receivers.h"
#include "Triggers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AReceivers::AReceivers()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  RootComponent = CreateDefaultSubobject< USceneComponent>(TEXT("DefaultSceneRoot"));
}

// Called when the game starts or when spawned
void AReceivers::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AReceivers::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Call the actual reaction with the countdown timer or immediately execute it if the delay time is zero
void AReceivers::Activate()
{
  if (DelayTime != 0.0f)
    GetWorldTimerManager().SetTimer(DelayTimeHandler, this, &AReceivers::ReactMaster, DelayTime, false);
  else
    ReactMaster();
}

// The actual reaction function
void AReceivers::ReactMaster()
{
	React();
  if (IsValid(ReceiverSFX))
    UGameplayStatics::PlaySoundAtLocation(this, ReceiverSFX, GetActorLocation());
}

