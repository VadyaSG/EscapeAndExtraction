// Copyright by VadyaS


#include "Weapon.h"
#include "Components/SphereComponent.h"
#include "MainCharacter.h"

AWeapon::AWeapon()
{
 	
	PrimaryActorTick.bCanEverTick = false;
	collision_sphere = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
	RootComponent = collision_sphere;
	collision_sphere->SetSphereRadius(pickup_rudius);

	collision_sphere->SetGenerateOverlapEvents(true);
	collision_sphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	collision_sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	pickup_mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	pickup_mesh->SetupAttachment(RootComponent);
	pickup_mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	collision_sphere->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::on_overlap_begin);
}

void AWeapon::on_overlap_begin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	AMainCharacter* player = Cast<AMainCharacter>(OtherActor);
	if (!player)return;
	if (!player->IsLocallyControlled()) return;

	UHotBar* hotbar = player->micro_inventory;
		if (hotbar)
		{
			bool is_picked_up = hotbar->add_item(item_data);

			if (is_picked_up)
			{
				Destroy();
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, TEXT("Inventory is Full"));
			}
		}

}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

