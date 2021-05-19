// Fill out your copyright notice in the Description page of Project Settings.


#include "Receiver_CustomEvent.h"

// Call the custom blueprint event
void AReceiver_CustomEvent::React()
{
  CustomReactEvent();

  IsActivated = !IsActivated;
}