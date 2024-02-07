// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	OriginalLocation = GetOwner()->GetActorLocation();
	Speed = MoveOffset.Length() / Duration;
	SetMoveInitialValues();
}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Move(DeltaTime);
}

void UMover::SetCanMove(bool bCanMoveNewValue)
{
	bCanMove = bCanMoveNewValue;
}

void UMover::SetMoveInitialValues(bool bReverseMove)
{
	//UE_LOG(LogTemp, Display, TEXT("bReverseMove: %s."), bReverseMove ? TEXT("true") : TEXT("false"));
	//UE_LOG(LogTemp, Display, TEXT("MoveOffset: %s."), *MoveOffset.ToString());
	
	TargetLocation = OriginalLocation;
	
	if (bReverseMove) return;
	
	TargetLocation += MoveOffset;
}

void UMover::Move(float DeltaTime)
{
	if (bCanMove)
	{
		AActor* Owner = GetOwner();
		FVector CurrentLocation = Owner->GetActorLocation();

		FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
		Owner->SetActorLocation(NewLocation);

		bCanMove = FVector::Distance(NewLocation, TargetLocation) > 0;
	}
}