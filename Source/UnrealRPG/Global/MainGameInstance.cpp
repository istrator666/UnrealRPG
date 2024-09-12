// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"
#include "DataTable/MonsterDataRow.h"

const FMonsterDataRow* UMainGameInstance::GetMonsterData(FName _Name)
{
    if (nullptr == MonsterDataTable)
    {
        return nullptr;
    }

    FMonsterDataRow* MonsterData = MonsterDataTable->FindRow<FMonsterDataRow>(_Name, nullptr);

    if (nullptr == MonsterData)
    {
        return nullptr;
    }

    return MonsterData;
}