//copyright by VadyaS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HotBar.h"
#include "MainCharacter.generated.h"


class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UStaminaComponent;
class UHealthComponent;
class UAnimMontage;
class UHotBar;

UCLASS()
class ESCAPEANDEXTRACTION_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	AMainCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	int32 get_active_slot_index()const { return active_slot_index; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* input_mapping_context;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* move_action;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* look_action;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* sprint_action;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* attack_action;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* hotbar_slot_action;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* reload_action;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* hand_attack_animation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UHotBar* micro_inventory;


protected:
	
	virtual void BeginPlay() override;
	UFUNCTION()
	void move(const FInputActionValue& value);

	UFUNCTION()
	void look(const FInputActionValue& value);

	UFUNCTION()
	void start_sprint();

	UFUNCTION()
	void stop_sprint();

	UFUNCTION()
	void start_attack();

	UFUNCTION()
	void stop_attack();


	UFUNCTION()
	void select_hotbar_slot(const FInputActionValue& value);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float forward_walk_speed = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float backward_walk_speed = 150.f;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Movement")
	float running_speed = 600.f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool is_sprinting = false;

	UFUNCTION()
	void on_stamina_emty();

	UFUNCTION()
	void reload();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* spring_arm_comp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* camera_comp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaminaComponent* stamina_component;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UHealthComponent* health_component;

	UPROPERTY(BlueprintReadOnly, Category = "Animations|Combat")
	EWeaponGripType current_grip_type = EWeaponGripType::Unarmed;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	int32 active_slot_index = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Animations|Combat")
	float aim_pitch = 0.f;

	
private:
	float current_forward_input = 0;
	float last_scroll_time = 0.f;

	UFUNCTION()
	void death_handle();

	UPROPERTY()
	AActor* cuurent_equipped_item = nullptr;

};
