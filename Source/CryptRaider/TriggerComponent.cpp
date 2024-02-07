// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Mover) return;

	AActor* UnlockableActor = GetUnlockableActor();
	if (!UnlockableActor)
	{
		if (bHasOverlappingUnlockableActor)
		{
			bHasOverlappingUnlockableActor = false;
			Mover->SetMoveInitialValues(true);
			
			if (bReverseMoverOnNullUnlockableActor)
			{
				Mover->SetCanMove(true);
			}
		}
		return;
	}
		
	UPrimitiveComponent* CastedUnlockableActor = Cast<UPrimitiveComponent>(UnlockableActor->GetRootComponent());
	if (CastedUnlockableActor)
	{
		CastedUnlockableActor->SetSimulatePhysics(false);
	}

	UnlockableActor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
	Mover->SetMoveInitialValues();
	Mover->SetCanMove(true);
}

void UTriggerComponent::SetMover(UMover* NewMover)
{
	Mover = NewMover;
}

AActor* UTriggerComponent::GetUnlockableActor()
{
	//Search about TInlineAllocator
	TArray<AActor*> OverlappingActorArray;
	GetOverlappingActors(OverlappingActorArray);

	for (AActor* Actor : OverlappingActorArray)
	{
		if (Actor->ActorHasTag(UnlockableTag) && !Actor->ActorHasTag(grabbedTag))
		{
			bHasOverlappingUnlockableActor = true;
			return Actor;
		}
	}

	return nullptr;
}