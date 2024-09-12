// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNREALRPG_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	const struct FMonsterDataRow* GetMonsterData(FName _Name);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UDataTable* MonsterDataTable;
	
};
