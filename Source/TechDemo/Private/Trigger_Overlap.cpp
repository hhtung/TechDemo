// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger_Overlap.h"
#include "TechDemoCharacter.h"

// Initial collision settings
ATrigger_Overlap::ATrigger_Overlap()
{
  InteractiveRange = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
  InteractiveRange->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
  InteractiveRange->InitBoxExtent(FVector(100.0f, 100.0f, 100.0f));
  InteractiveRange->OnComponentBeginOverlap.AddDynamic(this, &ATrigger_Overlap::OnTriggerOverlap);
  InteractiveRange->OnComponentEndOverlap.AddDynamic(this, &ATrigger_Overlap::OnTriggerExit);
}

void ATrigger_Overlap::OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  // call the activation function when overlapping with the player character
  if (OtherActor->IsA(ATechDemoCharacter::StaticClass()))
      CallReceivers();
}

void ATrigger_Overlap::OnTriggerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
  // if can be triggered multiple times, 
  // call the activation function again when the player character leaves the ara
  if (OtherActor->IsA(ATechDemoCharacter::StaticClass()))
  {
    if (!IsTriggerOnce)
      CallReceivers();
  }
}
