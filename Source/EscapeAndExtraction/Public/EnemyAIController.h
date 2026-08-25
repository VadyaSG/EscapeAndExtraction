// Copyright by VadyaS

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"


UENUM(BlueprintType)
enum class EAIState :uint8
{
	Patrol,
	Chase,
	Attack
};

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UBehaviorTree;

UCLASS()
class ESCAPEANDEXTRACTION_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyAIController();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* AI_perception_comp;

	UAISenseConfig_Sight* sight_config;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* behavor_tree;

	UFUNCTION()
	void on_target_perceived(AActor* actor, FAIStimulus stimuls);
};
