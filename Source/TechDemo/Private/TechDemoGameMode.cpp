// Copyright Epic Games, Inc. All Rights Reserved.

#include "TechDemoGameMode.h"
#include "TechDemoCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATechDemoGameMode::ATechDemoGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
