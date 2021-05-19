// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Receivers.h"
#include "Receiver_TransformModifier.generated.h"


USTRUCT(BlueprintType)
struct FTransformElements
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ReceiverGroup)
    AActor* Object;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ReceiverGroup)
    FTransform Origin;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ReceiverGroup)
    FTransform Delta;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ReceiverGroup)
    FTransform Destination;

  TArray<UMeshComponent*> Meshes;
};

UCLASS()
class TECHDEMO_API AReceiver_TransformModifier : public AReceivers
{
	GENERATED_BODY()

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

public:
  // Editor button function to record targets' original transform data
  UFUNCTION(CallInEditor, Category = Interactive)
    void SetOrigin();

  // Editor button function to record the change of the targets' transform data
  UFUNCTION(CallInEditor, Category = Interactive)
    void SetDelta();

  // Editor button function to set targets transform to the recorded original  data
  UFUNCTION(CallInEditor, Category = Interactive)
    void CheckOrigin();

  // Editor button function to set targets transform with the recorded changing data
  UFUNCTION(CallInEditor, Category = Interactive)
    void CheckDestination();

  // The duration time for modifying the targets' transform
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interactive)
    float MovingTime = 3.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interactive)
    TArray< FTransformElements > Targets;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Interactive)
    bool IsOutlinePoped = false;

  // Overriding activating function
  virtual void React() override;

  // Blueprint event for actual modification
  UFUNCTION(BlueprintImplementableEvent)
    void MoveTargets();

  // Blueprint event for actual modification (for reverse)
  UFUNCTION(BlueprintImplementableEvent)
    void ReverseTargets();

  UFUNCTION()
    void ToggleOutLine(bool ShouldPopOutline);
};
