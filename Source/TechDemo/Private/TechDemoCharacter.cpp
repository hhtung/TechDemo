// Copyright Epic Games, Inc. All Rights Reserved.

#include "TechDemoCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Triggers.h"
#include "FocusableOBJ.h"
#include "Trigger_Focus.h"

//////////////////////////////////////////////////////////////////////////
// ATechDemoCharacter
ATechDemoCharacter::ATechDemoCharacter()
{
  PrimaryActorTick.bCanEverTick = true;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

  TraceParams = FCollisionQueryParams(FName(TEXT("Trace")), true, this);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input
void ATechDemoCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

  PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ATechDemoCharacter::Interact);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATechDemoCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATechDemoCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ATechDemoCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ATechDemoCharacter::LookUpAtRate);
}

void ATechDemoCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATechDemoCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ATechDemoCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ATechDemoCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ATechDemoCharacter::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
  ViewCheck();
}

// Call the trigger to activate if currently viewing an focus trigger
// or overlap with a non-focus trigger
void ATechDemoCharacter::Interact()
{
	if (IsValid(InteractingTrigger))
	{
		InteractingTrigger->CallReceivers();
	}

  else if (IsValid(FocusingTrigger))
  {
		FocusingTrigger->CallReceivers();
  }
}

void ATechDemoCharacter::ViewCheck()
{
  // Check if any object in view
  AActor* CurrentFocusingOBJ = SearchFocusingOBJ();

  // if there is an object in view
	if (CurrentFocusingOBJ)
	{
    // if current focusing object is not the one last focused,
		// hide the outline if necessary and reset the focusing trigger pointer
		if (FocusingOBJ)
    {
			if (CurrentFocusingOBJ != FocusingOBJ)
			{
				if (FocusingOBJ->IsA(AFocusableOBJ::StaticClass()))
          Cast< AFocusableOBJ>(FocusingOBJ)->Trigger->PopOutLine(false);

				else if (FocusingOBJ->IsA(ATriggers::StaticClass()))
          Cast< ATriggers>(FocusingOBJ)->PopSelfOutLine(false);

				FocusingTrigger = nullptr;
			}
    }

    FocusingOBJ = CurrentFocusingOBJ;
		
		// if currently focusing at an interactive object, pop the outline
		// and set its receiver as self's focusing trigger if its trigger has the property
		if (CurrentFocusingOBJ->IsA(AFocusableOBJ::StaticClass()))
		{
			ATriggers* CurrentTrigger = Cast<AFocusableOBJ>(CurrentFocusingOBJ)->Trigger;
			if (CurrentTrigger->ShouldPopOutLine)
				FocusingTrigger = CurrentTrigger;
      CurrentTrigger->PopOutLine(true);
		}

    // if currently focusing at an trigger and its meshes should be outlined, pop the outline
    // and set it as self's focusing trigger
		else if (CurrentFocusingOBJ->IsA(ATriggers::StaticClass()))
		{
			ATriggers* CurrentTrigger = Cast< ATriggers>(CurrentFocusingOBJ);
			FocusingTrigger = CurrentTrigger;
			CurrentTrigger->PopSelfOutLine(true);
		}
	}

  // if there is no object in view, hide the previous focusing object and trigger's outline
	// and reset the pointers
	else if (FocusingOBJ)
	{
    if (FocusingOBJ->IsA(AFocusableOBJ::StaticClass()))
      Cast< AFocusableOBJ>(FocusingOBJ)->Trigger->PopOutLine(false);

    else if (FocusingOBJ->IsA(ATriggers::StaticClass()))
      Cast< ATriggers>(FocusingOBJ)->PopSelfOutLine(false);

		FocusingTrigger = nullptr;
		FocusingOBJ = nullptr;
	}
}

// Get the line trace result
AActor* ATechDemoCharacter::SearchFocusingOBJ()
{
  FVector Loc;
  FRotator Rot;
  FHitResult Hit(ForceInit);
  GetController()->GetPlayerViewPoint(Loc, Rot);

  FVector Start = Loc;
  FVector End = Start + (Rot.Vector() * ViewTraceLength);

  GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

  return Hit.GetActor();
}