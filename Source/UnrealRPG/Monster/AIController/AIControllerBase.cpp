// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AIController/AIControllerBase.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISense_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"

AAIControllerBase::AAIControllerBase()
{
	// Perception Component 생성
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	// Sight Configuration 설정
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
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

	// AI가 Actor를 감지할 때 호출될 함수 바인딩
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAIControllerBase::OnTargetDected);

}

void AAIControllerBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAIControllerBase::OnTargetDected(AActor* _Actor, FAIStimulus _Stimulus)
{
	// AI가 Actor를 감지했을 때
	if (_Stimulus.WasSuccessfullySensed())
	{
		// 감지한 Actor가 플레이어 캐릭터일 경우
		ACharacter* DetectedCharacter = Cast<ACharacter>(_Actor);
		if (DetectedCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player Detected: %s"), *DetectedCharacter->GetName());

			// 감지된 플레이어를 추적
			SetFocus(DetectedCharacter);
			MoveToActor(DetectedCharacter);
		}
	}
	else
	{
		// 플레이어를 잃었을 때
		UE_LOG(LogTemp, Warning, TEXT("Player Lost"));
		ClearFocus(EAIFocusPriority::Gameplay);
		StopMovement();
	}
}
