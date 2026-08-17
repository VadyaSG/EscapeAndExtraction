// Copyright by VadyaS

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HotBar.generated.h"

UENUM(BlueprintType)
enum class EWeaponGripType : uint8
{
	Unarmed     UMETA(DisplayName = "Unarmed"),
	Rifle       UMETA(DisplayName = "Rifle"),
	Pistol      UMETA(DisplayName = "Pistol")
};

USTRUCT(BlueprintType)
struct FHotbarItemSlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FName item_name = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	UTexture2D* item_icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int32 amount=0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TSubclassOf<AActor>item_class = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Item")
	EWeaponGripType grip_type = EWeaponGripType::Unarmed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int32 clip_ammo = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int32 ammo_in_inventory = 0;

	bool is_emty() const { return item_name.IsNone() || amount <= 0; }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHotbarChanged);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEANDEXTRACTION_API UHotBar : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHotBar();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnHotbarChanged on_hotbar_changed;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool add_item(FHotbarItemSlot new_item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void use_item_by_index(int32 index);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	const TArray<FHotbarItemSlot>& get_all_slots() const { return hotbar_slots; }

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void update_ammo_by_index(int32 index, int32 new_clip, int32 all_ammo);

	int32 get_slot_count()const { return slot_count; }
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory",meta = (ClampMin = "1", ClampMax = "9"))
	int32 slot_count = 5;

private:
	UPROPERTY()
	TArray<FHotbarItemSlot> hotbar_slots;
};
