// Copyright by VadyaS


#include "BaseEnemy.h"
#include "HealthComponent.h"

ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = false;
	health_component = CreateDefaultSubobject<UHealthComponent>("EnemyHealth");
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ABaseEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float actual_damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (actual_damage > 0.f && health_component)
	{
		health_component->apply_damage(actual_damage);

		if (health_component->is_dead())
		{
			death_handle();
		}
	}
	return actual_damage;
}

void ABaseEnemy::death_handle()
{
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));

	SetLifeSpan(6.f);
}