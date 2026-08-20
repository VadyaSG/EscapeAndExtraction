// Copyright by VadyaS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HotBar.h"
#include "Weapon.generated.h"

class USphereComponent;

UCLASS()
class ESCAPEANDEXTRACTION_API APickUpObj : public AActor
{
	GENERATED_BODY()
	
public:	
	
	APickUpObj();
	virtual void Tick(float DeltaTime) override;

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* collision_sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* pickup_mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Data")
	FHotbarItemSlot item_data;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = " Component")
	float pickup_rudius = 100.f;

	UFUNCTION()
	void on_overlap_begin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult);
};
