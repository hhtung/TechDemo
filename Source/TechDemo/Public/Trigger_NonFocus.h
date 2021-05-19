// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Triggers.h"
#include "Components/SphereComponent.h"
#include "Trigger_NonFocus.generated.h"

/**
 * 
 */
UCLASS()
class TECHDEMO_API ATrigger_NonFocus : public ATriggers
{
	GENERATED_BODY()

  UFUNCTION()
    void OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

  UFUNCTION()
    void OnTriggerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

  class ATechDemoCharacter* player;

public:
  ATrigger_NonFocus();

  UPROPERTY(EditAnywhere, Category = Interactive)
    USphereComponent* InteractiveRange;

  UPROPERTY(VisibleAnywhere, Category = Interactive)
    bool IsInRange = false;
};
