// Fill out your copyright notice in the Description page of Project Settings.


#include "FocusableOBJ.h"
#include "Triggers.h"

// Sets default values
AFocusableOBJ::AFocusableOBJ()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFocusableOBJ::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFocusableOBJ::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}