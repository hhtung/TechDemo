// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Triggers.h"
#include "Components/BoxComponent.h"
#include "Trigger_Overlap.generated.h"

/**
 * 
 */
UCLASS()
class TECHDEMO_API ATrigger_Overlap : public ATriggers
{
	GENERATED_BODY()

  UFUNCTION()
    void OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

  UFUNCTION()
    void OnTriggerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
  
  UBoxComponent* InteractiveRange;

public:
  ATrigger_Overlap();
};
