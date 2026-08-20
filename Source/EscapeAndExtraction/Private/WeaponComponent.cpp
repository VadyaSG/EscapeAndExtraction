// Copyright by VadyaS


#include "WeaponComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"


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
	if (current_ammo_in_magazine <= 0 )
	{
		if (ammo_in_inventory > 0)
		{
			reload();
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
		}
		return;
	}

	--current_ammo_in_magazine;

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

	FVector start_point = spawn_location;
	FVector forward_vector = spawn_rotation.Vector();
	FVector end_point = start_point + (forward_vector * fire_range);

	FHitResult hit_result;
	FCollisionQueryParams trace_params;

	if (GetOwner()->GetOwner())
	{
		trace_params.AddIgnoredActor(GetOwner());
	}
	
	bool hit = GetWorld()->LineTraceSingleByChannel(hit_result, start_point, end_point, ECC_Visibility, trace_params);

	DrawDebugLine(GetWorld(), start_point, end_point, FColor::Red, false, 1.f, 0, 1.f);

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
	if (current_ammo_in_magazine == max_magazine_capacity )return;

	int32 need_ammo_to_reload = max_magazine_capacity - current_ammo_in_magazine;
	int32 ammo_to_load = FMath::Min(need_ammo_to_reload, ammo_in_inventory);
	current_ammo_in_magazine += ammo_to_load;
	ammo_in_inventory -= ammo_to_load;
	on_ammo_changed.Broadcast(current_ammo_in_magazine, ammo_in_inventory);
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
		reload();
	}
	if (current_ammo_in_magazine <= 0&&ammo_in_inventory<=0)
	{
		stop_fire();
		return;	
	}
	

	fire(last_camera_loc, last_camera_rot);
}