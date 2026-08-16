// Copyright by VadyaS


#include "StaminaComponent.h"

UStaminaComponent::UStaminaComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;
}


void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();
	current_stamina = max_stamina;
	on_stamina_changed.Broadcast(1.f);
}


void UStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	bool is_stamina_changed = false;

	if (is_running)
	{
		current_stamina -= stamina_drain_rate * DeltaTime;
		is_stamina_changed = true;

		if (current_stamina <= 0)
		{
			current_stamina = 0.f;
			is_running = false;
			on_stamina_emty.Broadcast();
		}
	}
	else if (current_stamina < max_stamina)
	{
		current_stamina += stamina_regen_rate * DeltaTime;
		current_stamina = FMath::Clamp(current_stamina, 0.f, max_stamina);
		is_stamina_changed = true;
	}

	if (is_stamina_changed && max_stamina > 0.f)
	{
		float percent = current_stamina / max_stamina;
		on_stamina_changed.Broadcast(percent);
	}
}

