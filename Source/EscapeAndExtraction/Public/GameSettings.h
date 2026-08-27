// Copyright by VadyaS

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, BlueprintType, meta = (Category = "Game", DisplayName = "Gameplay Configuration"))
class ESCAPEANDEXTRACTION_API UGameSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UGameSettings();
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Accessibility", meta = (ConfigRestartRequired = false))
	float mouse_sensivity;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Gameplay|Timings")
	float spawn_delay;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Gameplay|Timings")
	float key_card_ttl;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Gameplay|Balancing")
	float enemy_modification;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Gameplay|Balancing")
	float stamina_drain_rate;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Gameplay|Balancing")
	float stamina_regen_rate;

	UFUNCTION(BlueprintPure, Category = "Gameplay", meta = (DisplayName = "Get Enemy Modification"))
	static float GetEnemyModificationFactor();

}; 


