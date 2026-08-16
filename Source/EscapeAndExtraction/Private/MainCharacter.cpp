//copyright by VadyaS


#include "MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "PlayerControllerMain.h"


AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	spring_arm_comp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	spring_arm_comp->SetupAttachment(GetRootComponent());

	camera_comp = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	camera_comp->SetupAttachment(spring_arm_comp);

}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	
	if (APlayerController* player_controller = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(player_controller->GetLocalPlayer()))
		{
			subsystem->AddMappingContext(input_mapping_context, 0);
		}
	}
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(move_action, ETriggerEvent::Triggered, this, &AMainCharacter::move);
	}
}

void AMainCharacter::move(const FInputActionValue& value)
{
	const FVector2D movement_vector = value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(FVector::ForwardVector, movement_vector.X);
		AddMovementInput(FVector::RightVector, movement_vector.Y);
	}
}
