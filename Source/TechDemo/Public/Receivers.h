// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "Receivers.generated.h"

UCLASS(Abstract)
class TECHDEMO_API AReceivers : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AReceivers();

	void ReactMaster();

  FTimerHandle DelayTimeHandler;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Interface for the trigger to call
  UFUNCTION()
    void Activate();

	// Virtual function for the child class to implement the actual reaction
  virtual void React() {}

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Interactive)
    bool IsActivated = false;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Interactive)
		class ATriggers* Trigger;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interactive)
		float DelayTime;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interactive)
    USoundCue* ReceiverSFX;
};
