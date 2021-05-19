// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Receivers.h"
#include "Receiver_CustomEvent.generated.h"

/**
 * 
 */
UCLASS()
class TECHDEMO_API AReceiver_CustomEvent : public AReceivers
{
	GENERATED_BODY()
	
public:
	virtual void React() override;

  UFUNCTION(BlueprintImplementableEvent)
		void CustomReactEvent();
};
