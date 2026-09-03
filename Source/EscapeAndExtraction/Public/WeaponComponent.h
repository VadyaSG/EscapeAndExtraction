// Copyright by VadyaS

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

class USoundBase;
class UNiagaraSystem;
class AMainCharacter;
class USoundBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChanged, int32, current_ammo, int32, max_ammo);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ESCAPEANDEXTRACTION_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UWeaponComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void start_fire(const FVector& camera_loc, const FRotator& camera_rot);
	void stop_fire();

	FVector last_camera_loc;
	FRotator last_camera_rot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Ammo")
	int32 current_ammo_in_magazine = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Ammo")
	int32 max_magazine_capacity = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Ammo")
	int32 ammo_in_inventory = 90;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Ammo")
	float fire_range = 5000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Ammo")
	float weapon_damage = 25.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Ammo")
	float fire_rate = 600.f;

	UPROPERTY(BlueprintAssignable, Category = "Weapon | Events")
	FOnAmmoChanged on_ammo_changed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon |sounds")
	USoundBase* shoot_sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon |VFX")
	UNiagaraSystem* start_shoot_flash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon |VFX")
	UNiagaraSystem* tracer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon |Animation")
	UAnimMontage* reload_animation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon |Animation")
	USoundBase* reload_sound;

	UFUNCTION(BlueprintCallable, Category = "Weapon | Actions")
	void fire(const FVector& spawn_location, const FRotator& spawn_rotation);

	UFUNCTION(BlueprintCallable, Category = "Weapon | Actions")
	void reload();

	void complete_reload_with_param(AMainCharacter* player_owner);

protected:
	virtual void BeginPlay() override;
	FTimerHandle fire_timer_handle;
	void shot_bullet();

};
