// Fill out your copyright notice in the Description page of Project Settings.


#include "Receiver_TransformModifier.h"
#include "FocusableOBJ.h"

void AReceiver_TransformModifier::BeginPlay()
{
  Super::BeginPlay();
  
  for (auto& target : Targets)
  {
    target.Object->GetComponents<UMeshComponent>(target.Meshes);

    // Safety assign self's trigger to the targets if they are interactive objects
    if (target.Object->IsA(AFocusableOBJ::StaticClass()))
      Cast<AFocusableOBJ>(target.Object)->Trigger = this->Trigger;
  }

  // Set the stencil value of the targets' mesh
  for (auto& target : Targets)
  {
    for (UMeshComponent* mesh : target.Meshes)
      mesh->SetCustomDepthStencilValue(50.0f);
  }
}

// Record target's original transform
void AReceiver_TransformModifier::SetOrigin()
{
  for (auto& target : Targets)
  {
    if (IsValid(target.Object))
    {
      target.Origin = target.Object->GetActorTransform();
    }
  }
}

// Record the change of target's transform
void AReceiver_TransformModifier::SetDelta()
{
  for (auto& target : Targets)
  {
    if (IsValid(target.Object))
    {
      target.Delta.SetLocation(target.Object->GetActorLocation() - target.Origin.GetLocation());
      target.Delta.SetRotation(target.Object->GetActorRotation().Quaternion() - target.Origin.GetRotation());
      target.Delta.SetScale3D(target.Object->GetActorScale3D() - target.Origin.GetScale3D());
    }
  }
}

// Modify target's transform to the recorded initial data
void AReceiver_TransformModifier::CheckOrigin()
{
  for (auto& target : Targets)
  {
    if (IsValid(target.Object))
    {
      target.Object->SetActorTransform(target.Origin);
    }
  }
}

// Modify target's transform according to the recorded changing data
void AReceiver_TransformModifier::CheckDestination()
{
  for (auto& target : Targets)
  {
    if (IsValid(target.Object))
    {
      target.Destination.SetLocation(target.Origin.GetLocation() + target.Delta.GetLocation());
      target.Destination.SetRotation(target.Origin.GetRotation() + target.Delta.GetRotation());
      target.Destination.SetScale3D(target.Origin.GetScale3D() + target.Delta.GetScale3D());

      target.Object->SetActorTransform(target.Destination);
    }
  }
}

// Change the target's transform, if call this function again, the change will reverse
void AReceiver_TransformModifier::React()
{
  if (!IsActivated)
    MoveTargets();
  else
    ReverseTargets();

  IsActivated = !IsActivated;
}

// Toggle the targets' outline
void AReceiver_TransformModifier::ToggleOutLine(bool ShouldPopOutline)
{ 
  for (auto& target : Targets)
  {
    for (UMeshComponent* mesh : target.Meshes)
    {
      mesh->SetRenderCustomDepth(ShouldPopOutline);
    }
  }

  IsOutlinePoped = ShouldPopOutline;
}