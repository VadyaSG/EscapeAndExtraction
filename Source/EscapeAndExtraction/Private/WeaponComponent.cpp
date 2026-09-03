#include "WeaponComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "../../../../UE_5.8/Engine/Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Camera/CameraComponent.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	on_ammo_changed.Broadcast(current_ammo_in_magazine, ammo_in_inventory);
}

void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UWeaponComponent::fire(const FVector& spawn_location, const FRotator& spawn_rotation)
{
	if (current_ammo_in_magazine <= 0)
	{
		if (ammo_in_inventory > 0)
		{
			AActor* weapon_actor = GetOwner();
			if (weapon_actor)
			{
				AMainCharacter* player_owner = Cast<AMainCharacter>(weapon_actor->GetOwner());
				if (player_owner)
				{
					player_owner->reload();
				}
			}
		}
		return;
	}

	--current_ammo_in_magazine;

	if (shoot_sound && GetOwner())
	{
		UStaticMeshComponent* weapon_mesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
		if (weapon_mesh)
		{
			float random_pitch = FMath::FRandRange(0.95f, 1.05f);
			UGameplayStatics::SpawnSoundAttached(shoot_sound, weapon_mesh, TEXT("FireSocket"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true, 1.f, random_pitch);
		}

		if (start_shoot_flash)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(start_shoot_flash, weapon_mesh, TEXT("FireSocket"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
		}
	}

	AActor* weapon_actor = GetOwner();
	if (weapon_actor)
	{
		AMainCharacter* player_owner = Cast<AMainCharacter>(weapon_actor->GetOwner());
		if (player_owner && player_owner->micro_inventory)
		{
			int32 active_index = player_owner->get_active_slot_index();
			player_owner->micro_inventory->update_ammo_by_index(active_index, current_ammo_in_magazine, ammo_in_inventory);
		}
	}

	AActor* weapon_owner = GetOwner();
	if (weapon_owner)
	{
		AActor* instigator_actor = weapon_owner->GetOwner();
		if (instigator_actor)
		{
			instigator_actor->MakeNoise(1.0f, Cast<APawn>(instigator_actor), instigator_actor->GetActorLocation());
		}
	}

	FVector start_point = spawn_location;
	FVector forward_vector = spawn_rotation.Vector();
	FVector end_point = start_point + (forward_vector * fire_range);

	FHitResult hit_result;
	FCollisionQueryParams trace_params;

	if (GetOwner() && GetOwner()->GetOwner())
	{
		trace_params.AddIgnoredActor(GetOwner());

		if (AActor* character_owner = GetOwner()->GetOwner())
		{
			trace_params.AddIgnoredActor(character_owner);
		}
	}

	bool hit = GetWorld()->LineTraceSingleByChannel(hit_result, start_point, end_point, ECC_Visibility, trace_params);
	FVector blaster_end_point = hit ? hit_result.ImpactPoint : end_point;

	if (tracer && GetWorld())
	{
		UStaticMeshComponent* weapon_mesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
		FVector start_blast_location = spawn_location;
		FRotator start_blast_rotation = spawn_rotation;

		if (weapon_mesh && weapon_mesh->DoesSocketExist(TEXT("FireSocket")))
		{
			start_blast_location = weapon_mesh->GetSocketLocation(("FireSocket"));
			start_blast_rotation = (blaster_end_point - start_blast_location).Rotation();
		}

		UNiagaraComponent* tracer_comp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), tracer, start_blast_location, start_blast_rotation);
		if (tracer_comp)
		{
			tracer_comp->SetVectorParameter(TEXT("BlasterEnd"), blaster_end_point);
		}
	}

	if (hit && hit_result.GetActor())
	{
		DrawDebugPoint(GetWorld(), hit_result.ImpactPoint, 10.f, FColor::Green, false, 1.f);
		AActor* hit_actor = hit_result.GetActor();
		FDamageEvent damage_event;
		hit_actor->TakeDamage(weapon_damage, damage_event, nullptr, GetOwner());
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Orange, FString::Printf(TEXT("%s"), *hit_result.GetActor()->GetName()));
	}
}

void UWeaponComponent::reload()
{
	AActor* weapon_actor = GetOwner();
	if (!weapon_actor) return;

	AMainCharacter* player_owner = Cast<AMainCharacter>(weapon_actor->GetOwner());
	if (!player_owner) return;

	if (GetWorld())
	{
		UGameplayStatics::PlaySoundAtLocation(this, reload_sound, weapon_actor->GetActorLocation());
		FTimerHandle ReloadTimerHandle;
		FTimerDelegate ReloadDelegate;
		ReloadDelegate.BindUObject(this, &UWeaponComponent::complete_reload_with_param, player_owner);

		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, ReloadDelegate, 1.5f, false);
	}
}

void UWeaponComponent::complete_reload_with_param(AMainCharacter* player_owner)
{

	if (player_owner)
	{
		player_owner->set_is_reload(false);
	}

	if (!player_owner || current_ammo_in_magazine == max_magazine_capacity || ammo_in_inventory <= 0) return;

	int32 need_ammo_to_reload = max_magazine_capacity - current_ammo_in_magazine;
	int32 ammo_to_load = FMath::Min(need_ammo_to_reload, ammo_in_inventory);
	current_ammo_in_magazine += ammo_to_load;
	ammo_in_inventory -= ammo_to_load;
	on_ammo_changed.Broadcast(current_ammo_in_magazine, ammo_in_inventory);

	if (player_owner->micro_inventory)
	{
		int32 active_index = player_owner->get_active_slot_index();
		player_owner->micro_inventory->update_ammo_by_index(active_index, current_ammo_in_magazine, ammo_in_inventory);
	}
	
}

void UWeaponComponent::start_fire(const FVector& camera_loc, const FRotator& camera_rot)
{
	if (GetWorld() == nullptr) return;
	last_camera_loc = camera_loc;
	last_camera_rot = camera_rot;

	shot_bullet();

	float time_between_shots = 60.f / FMath::Max(1.f, fire_rate);
	GetWorld()->GetTimerManager().SetTimer(fire_timer_handle, this, &UWeaponComponent::shot_bullet, time_between_shots, true);
}

void UWeaponComponent::stop_fire()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(fire_timer_handle);
	}
}

void UWeaponComponent::shot_bullet()
{
	if (current_ammo_in_magazine <= 0 && ammo_in_inventory > 0)
	{
		AActor* weapon_actor = GetOwner();
		if (weapon_actor)
		{
			AMainCharacter* player_owner = Cast<AMainCharacter>(weapon_actor->GetOwner());
			if (player_owner)
			{
				player_owner->reload();
			}
		}
	}
	if (current_ammo_in_magazine <= 0 && ammo_in_inventory <= 0)
	{
		stop_fire();
		return;
	}

	AActor* weapon_actor = GetOwner();
	if (!weapon_actor || !GetWorld()) return;

	AMainCharacter* player_owner = Cast<AMainCharacter>(weapon_actor->GetOwner());
	if (!player_owner || !player_owner->camera_comp) return;

	FVector camera_location = player_owner->camera_comp->GetComponentLocation();
	FVector camera_forward = player_owner->camera_comp->GetForwardVector();

	FVector safe_camera_start = camera_location + (camera_forward * 120.f);

	fire(safe_camera_start, camera_forward.Rotation());
}