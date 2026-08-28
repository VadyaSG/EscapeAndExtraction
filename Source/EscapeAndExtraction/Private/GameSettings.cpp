// Copyright by VadyaS


#include "GameSettings.h"

UGameSettings::UGameSettings()
{
	mouse_sensivity = 1.f;
	spawn_delay = 5.f;
	key_card_ttl = 30.f;
	enemy_modification = 1.f;
	stamina_drain_rate = 10.f;
	stamina_regen_rate = 8.f;
	min_spawn_delay = 0.5f;
	difficulty_step = 0.2f;
	update_difficulty_time = 10.f;
}

float UGameSettings::GetEnemyModificationFactor()
{
	const UGameSettings* Settings = GetDefault<UGameSettings>();
	return Settings ? Settings->enemy_modification : 1.0f;
}

float UGameSettings::get_key_kard_ttl()
{
	const UGameSettings* Settings = GetDefault<UGameSettings>();
	return Settings ? Settings->key_card_ttl : 30.0f;
}

float UGameSettings::get_spawn_delay()
{
	const UGameSettings* Settings = GetDefault<UGameSettings>();
	return Settings ? Settings->spawn_delay : 5.f;
}

float UGameSettings::get_min_spawn_delay()
{
	const UGameSettings* Settings = GetDefault<UGameSettings>();
	return Settings ? Settings->min_spawn_delay : 0.5f;
}

float UGameSettings::get_dificulty_step()
{
	const UGameSettings* Settings = GetDefault<UGameSettings>();
	return Settings ? Settings->difficulty_step : 0.2f;
}

float UGameSettings::get_update_difficulty_time()
{
	const UGameSettings* Settings = GetDefault<UGameSettings>();
	return Settings ? Settings->update_difficulty_time : 10.f;
}
