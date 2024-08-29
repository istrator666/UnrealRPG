// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AIController/AIControllerBase.h"

void AAIControllerBase::BeginPlay()
{
	Super::BeginPlay();
	this->MoveToLocation(FVector(0,0,0));
}
