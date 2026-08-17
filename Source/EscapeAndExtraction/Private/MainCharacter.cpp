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
#include "HotBar.h"
#include "WeaponComponent.h"



AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	spring_arm_comp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	spring_arm_comp->SetupAttachment(GetRootComponent());
	spring_arm_comp->bUsePawnControlRotation = true;
	
	camera_comp = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	camera_comp->SetupAttachment(spring_arm_comp);
	camera_comp->bUsePawnControlRotation = false;

	stamina_component = CreateDefaultSubobject<UStaminaComponent>("StaminaComponent");
	health_component = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	micro_inventory = CreateDefaultSubobject<UHotBar>("HotBar");
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

	if (Controller)
	{
		FRotator control_rot = GetControlRotation();
		float pitch = control_rot.Pitch;
		if (pitch > 180.f)
		{
			pitch -= 360.f;
		}
		aim_pitch = -pitch;
	}

	if (cuurent_equipped_item)
	{
		UWeaponComponent* weapon_comp =cuurent_equipped_item->FindComponentByClass<UWeaponComponent>();
		if (weapon_comp != nullptr && camera_comp != nullptr)
		{
			weapon_comp->last_camera_loc = camera_comp->GetComponentLocation();
			weapon_comp->last_camera_rot = camera_comp->GetComponentRotation();
		}
	}
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (move_action && look_action && sprint_action && attack_action && hotbar_slot_action && reload_action)
		{
			EnhancedInputComponent->BindAction(move_action, ETriggerEvent::Triggered, this, &AMainCharacter::move);
			EnhancedInputComponent->BindAction(look_action, ETriggerEvent::Triggered, this, &AMainCharacter::look);

			EnhancedInputComponent->BindAction(sprint_action, ETriggerEvent::Started, this, &AMainCharacter::start_sprint);
			EnhancedInputComponent->BindAction(sprint_action, ETriggerEvent::Completed, this, &AMainCharacter::stop_sprint);
			EnhancedInputComponent->BindAction(sprint_action, ETriggerEvent::Canceled, this, &AMainCharacter::stop_sprint);

			EnhancedInputComponent->BindAction(attack_action, ETriggerEvent::Started, this, &AMainCharacter::start_attack);
			EnhancedInputComponent->BindAction(attack_action, ETriggerEvent::Completed, this, &AMainCharacter::stop_attack);

			EnhancedInputComponent->BindAction(hotbar_slot_action, ETriggerEvent::Triggered, this, &AMainCharacter::select_hotbar_slot);

			
			EnhancedInputComponent->BindAction(reload_action, ETriggerEvent::Started, this, &AMainCharacter::reload);
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

void AMainCharacter::start_attack()
{
	if (health_component && health_component->is_dead()) return;
	if (is_sprinting)return;

	if (cuurent_equipped_item == nullptr)
	{
		if (hand_attack_animation)
		{
			PlayAnimMontage(hand_attack_animation);
		}
		return;
	}
	

	UWeaponComponent* weapon_comp = cuurent_equipped_item->FindComponentByClass<UWeaponComponent>();
	

	if (weapon_comp && camera_comp != nullptr && GetWorld() != nullptr)
	{
		FVector camera_location = camera_comp->GetComponentLocation();
		FRotator camera_rotation = camera_comp->GetComponentRotation();

		weapon_comp->start_fire(camera_location, camera_rotation);
	}
	
}

void AMainCharacter::select_hotbar_slot(const FInputActionValue& value)
{
	if (health_component && health_component->is_dead()) return;
	if (!micro_inventory && !GetWorld()) return;

	float slot_value = value.Get<float>();
	if (FMath::IsNearlyZero(slot_value)) return;

	if (cuurent_equipped_item != nullptr && micro_inventory != nullptr)
	{
		UWeaponComponent* old_weapon_comp = cuurent_equipped_item->FindComponentByClass<UWeaponComponent>();
		if (old_weapon_comp != nullptr)
		{
			micro_inventory->update_ammo_by_index(active_slot_index, old_weapon_comp->current_ammo_in_magazine, old_weapon_comp->ammo_in_inventory);
		}
	}

	int32 target_index = active_slot_index;

	if (slot_value >= 1.0f && slot_value <= micro_inventory->get_slot_count())
	{
		target_index = FMath::RoundToInt(slot_value) - 1;
	}
	else
	{
		float current_time = GetWorld()->GetTimeSeconds();
		if (current_time - last_scroll_time < 0.1f)
		{
			return;
		}
		last_scroll_time = current_time;

		if (slot_value > 0.f)
		{
			++target_index;
		}
		else
		{
			--target_index;
		}

		int32 max_slot = micro_inventory->get_slot_count();
		if (target_index >= max_slot) target_index = 0;
		if (target_index < 0) target_index = max_slot - 1;
	}

	active_slot_index = target_index;

	const TArray<FHotbarItemSlot>& current_slots = micro_inventory->get_all_slots();

	if (!current_slots.IsValidIndex(target_index)) return;
	FHotbarItemSlot target_slot = current_slots[target_index];

	if (target_slot.is_emty() || !target_slot.item_class)
	{
		if (cuurent_equipped_item)
		{
			cuurent_equipped_item->Destroy();
			cuurent_equipped_item = nullptr;
		}
		current_grip_type = EWeaponGripType::Unarmed;
		return;
	}

	if (cuurent_equipped_item)
	{
		cuurent_equipped_item->Destroy();
		cuurent_equipped_item = nullptr;
	}

	FActorSpawnParameters spawn_params;
	spawn_params.Owner = this;
	spawn_params.Instigator = GetInstigator();
	spawn_params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	cuurent_equipped_item = GetWorld()->SpawnActor<AActor>(target_slot.item_class, GetActorLocation(), GetActorRotation(), spawn_params);

	if (cuurent_equipped_item)
	{

		if (UPrimitiveComponent* RootPrimitive = Cast<UPrimitiveComponent>(cuurent_equipped_item->GetRootComponent()))
		{
			RootPrimitive->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			RootPrimitive->SetCollisionResponseToAllChannels(ECR_Ignore);
		}

		UWeaponComponent* new_weapon_comp = cuurent_equipped_item->FindComponentByClass<UWeaponComponent>();
		if (new_weapon_comp != nullptr)
		{
			new_weapon_comp->current_ammo_in_magazine = target_slot.clip_ammo;
			new_weapon_comp->ammo_in_inventory = target_slot.ammo_in_inventory;
		}

		FName socket_name = TEXT("RightHandWeaponSocket");
		if (target_slot.grip_type == EWeaponGripType::Pistol)
		{
			socket_name = TEXT("RightPistol");
		}

		cuurent_equipped_item->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, socket_name);
		current_grip_type = target_slot.grip_type;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Slot is emty!"));
	}
}


void AMainCharacter::reload()
{
	if (health_component && health_component->is_dead())return;

	if (cuurent_equipped_item == nullptr)return;

	UWeaponComponent* weapon_comp = cuurent_equipped_item->FindComponentByClass<UWeaponComponent>();

	if (weapon_comp&& weapon_comp->ammo_in_inventory>0)
	{
		weapon_comp->reload();

		int32 active_index = get_active_slot_index();
		micro_inventory->update_ammo_by_index(active_index, weapon_comp->current_ammo_in_magazine, weapon_comp->ammo_in_inventory);
	}
}

void AMainCharacter::stop_attack()
{
	if (cuurent_equipped_item == nullptr) return;

	UWeaponComponent* weapon_comp = cuurent_equipped_item->FindComponentByClass<UWeaponComponent>();
	if (weapon_comp != nullptr)
	{
		weapon_comp->stop_fire();
	}

}