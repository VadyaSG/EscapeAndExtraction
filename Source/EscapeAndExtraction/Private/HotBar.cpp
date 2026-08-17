// Copyright by VadyaS


#include "HotBar.h"

UHotBar::UHotBar()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UHotBar::BeginPlay()
{
	Super::BeginPlay();

	hotbar_slots.Empty();

	for (int32 i = 0; i < slot_count; ++i)
	{
		hotbar_slots.Add(FHotbarItemSlot());
	}
	on_hotbar_changed.Broadcast();
}


void UHotBar::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool UHotBar::add_item(FHotbarItemSlot new_item)
{
	if (new_item.is_emty())return false;

	for (int32 i = 0; i < hotbar_slots.Num(); ++i)
	{
		if (!hotbar_slots[i].is_emty() && hotbar_slots[i].item_name == new_item.item_name)
		{
			hotbar_slots[i].amount += new_item.amount;
			on_hotbar_changed.Broadcast();
			return true;
		}
	}
	for (int32 i = 0; i < hotbar_slots.Num(); ++i)
	{
		if (hotbar_slots[i].is_emty())
		{
			hotbar_slots[i] = new_item;
			on_hotbar_changed.Broadcast();
			return true;
		}
	}
	return false;
}

void UHotBar::use_item_by_index(int32 index)
{
	if (!hotbar_slots.IsValidIndex(index) || hotbar_slots[index].is_emty()) return;

	hotbar_slots[index].amount--;

	if (hotbar_slots[index].amount <= 0)
	{
		hotbar_slots[index] = FHotbarItemSlot();
	}
	on_hotbar_changed.Broadcast();
}

void UHotBar::update_ammo_by_index(int32 index, int32 new_clip, int32 all_ammo)
{
	if (hotbar_slots.IsValidIndex(index))
	{
		hotbar_slots[index].clip_ammo = new_clip;
		hotbar_slots[index].ammo_in_inventory = all_ammo;

		on_hotbar_changed.Broadcast();
	}
}