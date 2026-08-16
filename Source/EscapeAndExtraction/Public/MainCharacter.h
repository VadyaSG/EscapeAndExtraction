//copyright by VadyaS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* spring_arm_comp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* camera_comp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaminaComponent* stamina_component;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UHealthComponent* health_component;

	
private:
	float current_forward_input = 0;

	UFUNCTION()
	void death_handle();
	

};
