// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Mover.h"
#include "TriggerComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYPTRAIDER_API UTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()
	
public:
	UTriggerComponent();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetMover(UMover* NewMover);

	UPROPERTY(EditAnywhere)
	FName UnlockableTag = FName();

private:
	UPROPERTY()
	UMover* Mover = nullptr;

	UPROPERTY(EditAnywhere)
	bool bReverseMoverOnNullUnlockableActor = false;

	bool bHasOverlappingUnlockableActor = false;

	FName grabbedTag = FName(TEXT("Grabbed"));

	AActor* GetUnlockableActor();
};
