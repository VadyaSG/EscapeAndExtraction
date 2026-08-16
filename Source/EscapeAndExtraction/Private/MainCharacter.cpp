//copyright by VadyaS


#include "MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "PlayerControllerMain.h"
#include "StaminaComponent.h"
#include "HealthComponent.h"


AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	spring_arm_comp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	spring_arm_comp->SetupAttachment(GetRootComponent());
	
	camera_comp = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	camera_comp->SetupAttachment(spring_arm_comp);

	stamina_component = CreateDefaultSubobject<UStaminaComponent>("StaminaComponent");
	health_component = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
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

	if (stamina_component && health_component)
	{
		stamina_component->on_stamina_emty.AddDynamic(this, &AMainCharacter::on_stamina_emty);
		health_component->on_death.AddDynamic(this, &AMainCharacter::death_handle);
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
		if (move_action && look_action && sprint_action)
		{
			EnhancedInputComponent->BindAction(move_action, ETriggerEvent::Triggered, this, &AMainCharacter::move);
			EnhancedInputComponent->BindAction(look_action, ETriggerEvent::Triggered, this, &AMainCharacter::look);

			EnhancedInputComponent->BindAction(sprint_action, ETriggerEvent::Started, this, &AMainCharacter::start_sprint);
			EnhancedInputComponent->BindAction(sprint_action, ETriggerEvent::Completed, this, &AMainCharacter::stop_sprint);
			EnhancedInputComponent->BindAction(sprint_action, ETriggerEvent::Canceled, this, &AMainCharacter::stop_sprint);
		}
	
	}

}

void AMainCharacter::move(const FInputActionValue& value)
{
	const FVector2D movement_vector = value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		current_forward_input = movement_vector.Y;
		if (is_sprinting && current_forward_input <= 0)
		{
			stop_sprint();
		}
		if (!is_sprinting)
		{
			if (movement_vector.Y < 0.f)
			{
				GetCharacterMovement()->MaxWalkSpeed = backward_walk_speed;
			}
			else
			{
				GetCharacterMovement()->MaxWalkSpeed = forward_walk_speed;
			}
		}
		
		const FRotator rotation = GetActorRotation();
		const FRotator yaw_rotation(0, rotation.Yaw, 0);
		const FVector forward_direction = FRotationMatrix(yaw_rotation).GetUnitAxis(EAxis::X);
		const FVector right_direction = FRotationMatrix(yaw_rotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(forward_direction, movement_vector.Y);
		AddMovementInput(right_direction, movement_vector.X);

		if (is_sprinting)
		{
			if (GetVelocity().SizeSquared() < 100.f)
			{
				stamina_component->set_is_running(false);
			}
			else
			{
				stamina_component->set_is_running(true);
			}
		}

	}
}

void AMainCharacter::look(const FInputActionValue& value)
{
	const FVector2D look_vector = value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(look_vector.X);
		AddControllerPitchInput(look_vector.Y);
	}
}

void AMainCharacter::start_sprint()
{
	
	if (stamina_component && stamina_component->can_running())
	{
		
		if (GetCharacterMovement() && current_forward_input > 0.f)
		{
			is_sprinting = true;
			GetCharacterMovement()->MaxWalkSpeed = running_speed;

			stamina_component->set_is_running(true);
		}
	}
	
}

void AMainCharacter::stop_sprint()
{

	if (stamina_component)
	{
		stamina_component->set_is_running(false);
	}

	if (GetCharacterMovement())
	{
		is_sprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = forward_walk_speed;
	}
}

void AMainCharacter::on_stamina_emty()
{
	stop_sprint();
}

void AMainCharacter::death_handle()
{
	if (APlayerController* pc = Cast<APlayerController>(GetController()))
	{
		DisableInput(pc);
	}

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	}

	if (GetMesh())
	{
		GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
		GetMesh()->SetSimulatePhysics(true);
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->DisableMovement();
		GetCharacterMovement()->StopMovementImmediately();
	}
}