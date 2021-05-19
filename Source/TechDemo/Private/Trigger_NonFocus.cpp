// Copyright Epic Games, Inc. All Rights Reserved.

#include "Trigger_NonFocus.h"
#include "TechDemoCharacter.h"
#include "Receivers.h"
#include "Receiver_TransformModifier.h"

// Initial collision settings
ATrigger_NonFocus::ATrigger_NonFocus()
{
  InteractiveRange = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
  InteractiveRange->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
  InteractiveRange->InitSphereRadius(200.0f);
  InteractiveRange->OnComponentBeginOverlap.AddDynamic(this, &ATrigger_NonFocus::OnTriggerOverlap);
  InteractiveRange->OnComponentEndOverlap.AddDynamic(this, &ATrigger_NonFocus::OnTriggerExit);

  ShouldPopOutLine = true;
}

// Assign self to be the interacting trigger of the player when player is in range
void ATrigger_NonFocus::OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  if (!IsInRange && OtherActor->IsA(ATechDemoCharacter::StaticClass()))
  {
    IsInRange = true;
    Cast<ATechDemoCharacter>(OtherActor)->InteractingTrigger = this;

    // Call PopOutline to handle self's outline or the interactive object's outline
    PopOutLine(true); 
  }
}

// Remove self from the player's interacting trigger when player leaves
void ATrigger_NonFocus::OnTriggerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
  if (IsInRange && OtherActor->IsA(ATechDemoCharacter::StaticClass()))
  {
    IsInRange = false;
    Cast<ATechDemoCharacter>(OtherActor)->InteractingTrigger = nullptr;

    // Call PopOutline to handle self's outline or the interactive object's outline
    PopOutLine(false); 
  }
}

