// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AIController/AIControllerBase.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AISenseConfig_Sight.h"

AAIControllerBase::AAIControllerBase()
{

	// AIPerceptionComponent ���� �� ����
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SetPerceptionComponent(*AIPerceptionComponent);

	// Sight Configuration ����
	UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1000.0f;              // AI �þ� �Ÿ�
	SightConfig->LoseSightRadius = 1200.0f;          // AI�� �÷��̾ �Ҵ� �Ÿ�
	SightConfig->PeripheralVisionAngleDegrees = 90.0f; // AI �þ� ����
	SightConfig->SetMaxAge(5.0f);                    // AI�� ������ �����ϴ� �ð�
	SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	// AI�� ���� �ý��ۿ� �ð��� �߰�
	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	// Perception Updated ��������Ʈ ���ε�
	AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AAIControllerBase::OnPerceptionUpdated);

	TargetKey = "TargetActor";

}

void AAIControllerBase::BeginPlay()
{
	Super::BeginPlay();

}

void AAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BlackboardData)
	{
		if (UseBlackboard(BlackboardData, BlackboardComponent))
		{
			if (BehaviorTree)
			{
				RunBehaviorTree(BehaviorTree);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("BehaviorTree is not assigned in %s"), *GetName());
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to initialize Blackboard in %s"), *GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BlackboardData is not assigned in %s"), *GetName());
	}
}

void AAIControllerBase::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	if (!BlackboardComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("BlackboardComponent is null in %s"), *GetName());
		return;
	}

	for (AActor* Actor : UpdatedActors)
	{
		FActorPerceptionBlueprintInfo Info;
		AIPerceptionComponent->GetActorsPerception(Actor, Info);

		for (const FAIStimulus& Stimulus : Info.LastSensedStimuli)
		{
			if (Stimulus.WasSuccessfullySensed())
			{
				Blackboard->SetValueAsObject(TargetKey, Actor);
				return;
			}
			else
			{
				Blackboard->ClearValue(TargetKey);
			}
		}
	}
}
