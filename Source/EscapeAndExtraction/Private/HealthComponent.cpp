// Copyright by VadyaS


#include "HealthComponent.h"


UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	current_hp = max_hp;

	on_health_changed.Broadcast(current_hp,max_hp);

	AActor* owner_component = GetOwner();
	if (owner_component)
	{
		owner_component->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnTakeAnyDamage);
	}

}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (is_dead())return;
	current_hp = FMath::Clamp(current_hp - damage, 0.f, max_hp);
	on_health_changed.Broadcast(current_hp,max_hp);

	if (is_dead())
	{
		on_death.Broadcast();
	}
}

bool UHealthComponent::is_dead()
{
	return current_hp<=0;
}

bool UHealthComponent::add_health(float new_health)
{
	if (is_dead() || is_health_full())return false;

	current_hp = FMath::Clamp(current_hp + new_health, 0.f, max_hp);
	on_health_changed.Broadcast(current_hp,max_hp);

	return true;
}

bool UHealthComponent::is_health_full() const
{
	return current_hp>=max_hp;
}