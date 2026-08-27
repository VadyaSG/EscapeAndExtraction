// Copyright by VadyaS

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaminaEmty);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaChanged, float, stamina_percent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEANDEXTRACTION_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UStaminaComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool can_running()const { return current_stamina > 0; }
	void set_is_running(bool runnig) { is_running = runnig; }

	UFUNCTION(BlueprintCallable)
	bool get_is_running()const { return is_running; }

	UPROPERTY(BlueprintAssignable, Category = "Stamina | Events")
	FOnStaminaEmty on_stamina_emty;

	UPROPERTY(BlueprintAssignable, Category = "Stamina | Events")
	FOnStaminaChanged on_stamina_changed;


protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats | Stamina", meta = (ClampMin = "0.0"))
	float max_stamina = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats | Stamina", meta = (ClampMin = "0.0"))
	float current_stamina = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats | Stamina", meta = (ClampMin = "0.0"))
	float stamina_drain_rate = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats | Stamina", meta = (ClampMin = "0.0"))
	float stamina_regen_rate = 8.f;

private:
	bool is_running = false;

	
};
