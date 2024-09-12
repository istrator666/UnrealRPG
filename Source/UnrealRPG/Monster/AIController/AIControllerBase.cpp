// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AIController/AIControllerBase.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISense_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"

AAIControllerBase::AAIControllerBase()
{
	// Perception Component ����
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	// Sight Configuration ����
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
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

	// AI�� Actor�� ������ �� ȣ��� �Լ� ���ε�
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAIControllerBase::OnTargetDected);

}

void AAIControllerBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAIControllerBase::OnTargetDected(AActor* _Actor, FAIStimulus _Stimulus)
{
	// AI�� Actor�� �������� ��
	if (_Stimulus.WasSuccessfullySensed())
	{
		// ������ Actor�� �÷��̾� ĳ������ ���
		ACharacter* DetectedCharacter = Cast<ACharacter>(_Actor);
		if (DetectedCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player Detected: %s"), *DetectedCharacter->GetName());

			// ������ �÷��̾ ����
			SetFocus(DetectedCharacter);
			MoveToActor(DetectedCharacter);
		}
	}
	else
	{
		// �÷��̾ �Ҿ��� ��
		UE_LOG(LogTemp, Warning, TEXT("Player Lost"));
		ClearFocus(EAIFocusPriority::Gameplay);
		StopMovement();
	}
}
