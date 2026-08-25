// Copyright by VadyaS


#include "BaseEnemy.h"
#include "HealthComponent.h"
#include "EnemyAIController.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"

ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = false;
	health_component = CreateDefaultSubobject<UHealthComponent>("EnemyHealth");

	hp_widget_comp = CreateDefaultSubobject<UWidgetComponent>("EnemyHealthWidget");
	hp_widget_comp->SetupAttachment(GetMesh(), TEXT("HeadTop_End"));

}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	if (hp_widget_comp)
	{
		hp_widget_comp->SetVisibility(false);
	}
	
}

void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ABaseEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float actual_damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if (hp_widget_comp && !hp_widget_comp->IsVisible())
	{
		hp_widget_comp->SetVisibility(true);
	}

	if (actual_damage > 0.f && health_component)
	{
		
		if (health_component->is_dead())
		{
			death_handle();
		}
	}
	return actual_damage;
}

void ABaseEnemy::death_handle()
{

	if (hp_widget_comp)
	{
		hp_widget_comp->DestroyComponent();
	}

	if (AEnemyAIController* ai_controller = Cast<AEnemyAIController>(GetController()))
	{
		ai_controller->StopMovement();
		ai_controller->UnPossess();
	}
	if (UCapsuleComponent* CapsuleComp = GetCapsuleComponent())
	{
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	}
	
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->AddImpulse(GetActorForwardVector() * -2000.f, NAME_None, true);

	SetLifeSpan(6.f);
}