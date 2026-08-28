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

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Gameplay|Balancing")
	float min_spawn_delay;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Gameplay|Balancing")
	float difficulty_step;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Gameplay|Balancing")
	float update_difficulty_time;

	UFUNCTION(BlueprintPure, Category = "Gameplay", meta = (DisplayName = "Get Enemy Modification"))
	static float GetEnemyModificationFactor();

	UFUNCTION(BlueprintPure, Category = "Gameplay", meta = (DisplayName = "Get TTLCard Modification"))
	static float get_key_kard_ttl();

	UFUNCTION(BlueprintPure, Category = "Timings", meta = (DisplayName = "Get Spawn Delay"))
	static float get_spawn_delay();

	UFUNCTION(BlueprintPure, Category = "Gameplay", meta = (DisplayName = "Get min spawn delay"))
	static float get_min_spawn_delay();

	UFUNCTION(BlueprintPure, Category = "Gameplay", meta = (DisplayName = "Get difficulty step"))
	static float get_dificulty_step();

	UFUNCTION(BlueprintPure, Category = "Gameplay", meta = (DisplayName = "Get update difficulty time"))
	static float get_update_difficulty_time();
}; 


