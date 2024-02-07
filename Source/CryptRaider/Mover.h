// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Mover.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UMover : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMover();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetCanMove(bool bCanMoveNewValue);
	void SetMoveInitialValues(bool bReverseMove = false);

private:
	
	UPROPERTY(EditAnywhere)
	FVector MoveOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	float Duration = 4;

	bool bCanMove = false;
	
	float Speed = 0;
	FVector OriginalLocation = FVector::ZeroVector;
	FVector TargetLocation = FVector::ZeroVector;

	void Move(float DeltaTime);
};