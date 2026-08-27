// Copyright by VadyaS


#include "GameSettings.h"

UGameSettings::UGameSettings()
{
	mouse_sensivity = 1.f;
	spawn_delay = 5.f;
	key_card_ttl = 60.f;
	enemy_modification = 1.f;
	stamina_drain_rate = 10.f;
	stamina_regen_rate = 8.f;
}

float UGameSettings::GetEnemyModificationFactor()
{
	const UGameSettings* Settings = GetDefault<UGameSettings>();
	return Settings ? Settings->enemy_modification : 1.0f;
}
