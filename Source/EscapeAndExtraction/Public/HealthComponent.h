// Copyright by VadyaS

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, current_hp, float, max_hp);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEANDEXTRACTION_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
	FOnDeath on_death;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged on_health_changed;

	UFUNCTION(BlueprintCallable)
	float get_curren_hp()const { return current_hp; }

	UFUNCTION(BlueprintCallable)
	bool is_dead();

	bool add_health(float new_health);
	bool is_health_full() const;

	UFUNCTION(BlueprintCallable)
	void set_curren_hp(float hp);
protected:
	
	virtual void BeginPlay() override;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats | Health", meta = (ClampMin = "0.0"))
	float max_hp=100.f;

private:
	float current_hp=0.f;

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

};
