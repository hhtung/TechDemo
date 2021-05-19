// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "Triggers.generated.h"

UCLASS(Abstract)
class TECHDEMO_API ATriggers : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATriggers();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

  TArray<UMeshComponent*> OutLinedMeshes;

  // Pop the outline of this trigger's meshes (if have any)
  virtual void PopSelfOutline(){}

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

  UFUNCTION(CallInEditor, Category = Interactive)
		void AssignTrigger();

  // Blueprint event for self feedback (e.g. rotate the lever)
  UFUNCTION(BlueprintImplementableEvent, Category = Interactive)
    void SelfReact();

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interactive)
    bool IsTriggered = false;

  // if this property is false, this trigger can be triggered multiple times
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interactive)
    bool IsTriggerOnce = true;

  // Whether should pop related interactive objects' outline
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interactive)
    bool ShouldPopOutLine = false;

  // Whether should pop the outline of self's meshes
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interactive)
    bool ShouldPopSelfOutLine = false;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interactive)
    USoundCue* TriggerSFX;

  UPROPERTY(EditAnywhere, Category = Interactive)
    TArray< class AReceivers* > Receivers;

  // Call receivers to react
  UFUNCTION()
    void CallReceivers();

  void PopOutLine(bool ShouldDisplayOutline);

  void PopSelfOutLine(bool ShouldDisplayOutline);
};
