// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MonsterDataRow.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct UNREALRPG_API FMonsterDataRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
	TSubclassOf<AActor> Monster;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	int HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float RunSpeed;
	
};
