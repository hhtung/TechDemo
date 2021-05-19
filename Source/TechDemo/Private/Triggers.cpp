// Fill out your copyright notice in the Description page of Project Settings.


#include "Triggers.h"
#include "Receivers.h"
#include "Receiver_TransformModifier.h"
#include "FocusableOBJ.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATriggers::ATriggers()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject< USceneComponent>(TEXT("DefaultSceneRoot"));
}

// Called when the game starts or when spawned
void ATriggers::BeginPlay()
{
	Super::BeginPlay();

  // Safety assign the trigger to its receiver and related interactive objects
	AssignTrigger();

  // Collect self's outline meshes and set their stencil value
  GetComponents<UMeshComponent>(OutLinedMeshes);
  for (auto& mesh : OutLinedMeshes)
  {
    mesh->SetCustomDepthStencilValue(50.0f);
  }
}

// Called every frame
void ATriggers::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called all the receivers to react
void ATriggers::CallReceivers()
{
	// activate the receivers if have not been triggered or can be triggered multiple times
  if (!IsTriggerOnce || !IsTriggered)
	{
    IsTriggered = !IsTriggered;

    for (auto& receiver : Receivers)
    {
      if (IsValid(receiver))
        receiver->Activate();
    }

    // execute self's reaction like rotating the lever
    SelfReact();

    if (IsValid(TriggerSFX))
      UGameplayStatics::PlaySoundAtLocation(this, TriggerSFX, GetActorLocation());
	}
	
  // hide related objects' outline, mainly for the overlap trigger
  else
	  PopOutLine(false);
}

void ATriggers::AssignTrigger()
{
	for (auto& receiver : Receivers)
	{
		// Assign self to related receivers
    receiver->Trigger = this;

		if (receiver->IsA(AReceiver_TransformModifier::StaticClass()))
		{
			AReceiver_TransformModifier* TransformModifier = Cast<AReceiver_TransformModifier>(receiver);

      // Assign self to related interactive objects
      for (auto& target : TransformModifier->Targets)
			{
				if (target.Object->IsA(AFocusableOBJ::StaticClass()))
				{
					Cast<AFocusableOBJ>(target.Object)->Trigger = this;
				}
			}
		}
	}
}

void ATriggers::PopOutLine(bool ShouldDisplayOutline)
{
	// Pop related interactive object's outline if this trigger is set to pop,
  // also check it has not been triggered or can be triggered multiple times
  if (ShouldPopOutLine && ShouldDisplayOutline && (!IsTriggerOnce || !IsTriggered))
  {
    for (auto& receiver : Receivers)
    {
      if (receiver->IsA(AReceiver_TransformModifier::StaticClass()))
      {
        Cast<AReceiver_TransformModifier>(receiver)->ToggleOutLine(true);
      }
    }
  }

  // Hide the objects' outline
  else if (!ShouldDisplayOutline)
  {
    for (auto& receiver : Receivers)
    {
      if (receiver->IsA(AReceiver_TransformModifier::StaticClass()))
      {
        Cast<AReceiver_TransformModifier>(receiver)->ToggleOutLine(false);
      }
    }
  }
}

// Handle the outline of self's meshes, similar to the function above
void ATriggers::PopSelfOutLine(bool ShouldDisplayOutline)
{
  if (ShouldPopSelfOutLine && ShouldDisplayOutline && (!IsTriggerOnce || !IsTriggered))
  {
    for (auto& mesh : OutLinedMeshes)
    {
      mesh->SetRenderCustomDepth(true);
    }
  }

  else if (!ShouldDisplayOutline)
  {
    for (auto& mesh : OutLinedMeshes)
    {
      mesh->SetRenderCustomDepth(false);
    }
  }
}