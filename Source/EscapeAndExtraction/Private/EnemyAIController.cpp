// Copyright by VadyaS


#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


AEnemyAIController::AEnemyAIController()
{
	AI_perception_comp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));
	sight_config = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightComp"));

	sight_config->SightRadius = 1500.f;
	sight_config->LoseSightRadius = 2000.f;
	sight_config->PeripheralVisionAngleDegrees = 120.f;
	sight_config->DetectionByAffiliation.bDetectEnemies = true;
	sight_config->DetectionByAffiliation.bDetectFriendlies = true;
	sight_config->DetectionByAffiliation.bDetectNeutrals = true;

	AI_perception_comp->ConfigureSense(*sight_config);
	AI_perception_comp->SetDominantSense(sight_config->GetSenseImplementation());
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	AI_perception_comp->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::on_target_perceived);
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (behavor_tree)
	{
		RunBehaviorTree(behavor_tree);

		if (UBlackboardComponent* BB = GetBlackboardComponent())
		{
			BB->SetValueAsEnum(TEXT("AIState"), (uint8)EAIState::Patrol);
		}
	}
}

void AEnemyAIController::on_target_perceived(AActor* actor, FAIStimulus stimuls)
{
	UBlackboardComponent* BB = GetBlackboardComponent();
	if (!BB || !actor)return;

	if (actor->ActorHasTag(TEXT("Player")))
	{
		if (stimuls.WasSuccessfullySensed())
		{
			BB->SetValueAsObject(TEXT("TargetActor"), actor);
			BB->SetValueAsEnum(TEXT("AIState"), (uint8)EAIState::Chase);
		}
		else
		{
			BB->ClearValue(TEXT("TargetActor"));
			BB->SetValueAsEnum(TEXT("AIState"), (uint8)EAIState::Patrol);
		}
	}
}
