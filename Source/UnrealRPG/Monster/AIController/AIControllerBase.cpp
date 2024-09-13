// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AIController/AIControllerBase.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AISenseConfig_Sight.h"

AAIControllerBase::AAIControllerBase()
{

	// AIPerceptionComponent 생성 및 설정
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SetPerceptionComponent(*AIPerceptionComponent);

	// Sight Configuration 설정
	UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1000.0f;              // AI 시야 거리
	SightConfig->LoseSightRadius = 1200.0f;          // AI가 플레이어를 잃는 거리
	SightConfig->PeripheralVisionAngleDegrees = 90.0f; // AI 시야 각도
	SightConfig->SetMaxAge(5.0f);                    // AI가 정보를 유지하는 시간
	SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	// AI의 감각 시스템에 시각을 추가
	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	// Perception Updated 델리게이트 바인딩
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
