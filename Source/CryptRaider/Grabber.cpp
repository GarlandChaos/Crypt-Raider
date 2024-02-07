// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("Physics Handle"));
	//UE_LOG(LogTemp, Log, TEXT("Is PhysicsHandle null? %s"), (PhysicsHandle == nullptr) ? TEXT("True") : TEXT("False"));
	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	PhysicsHandle = GetPhysicsHandle();
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* AttachedPhysicsHandle = GetOwner()->GetComponentByClass<UPhysicsHandleComponent>();

	if (!AttachedPhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("No UPhysicsHandleComponent found on owner object."));
	}

	return AttachedPhysicsHandle;
}

bool UGrabber::IsGrabbing() const
{
	if (!PhysicsHandle) return false;

	UPrimitiveComponent* GrabbedComponent = PhysicsHandle->GetGrabbedComponent();

	return GrabbedComponent != nullptr;
}

TOptional<FHitResult> UGrabber::GetGrabbableInReach() const
{
	FHitResult OutHitResult;
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * RaycastMaxDistance;
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(SweepSphereRadius);

	bool hasHit = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, SphereShape);

	return hasHit ? OutHitResult : TOptional<FHitResult>();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsGrabbing()) return;

	FVector newLocation = GetComponentLocation() + GetForwardVector() * GrabDistance;
	PhysicsHandle->SetTargetLocationAndRotation(newLocation, GetComponentRotation());
}

void UGrabber::Grab() 
{
	if (!PhysicsHandle) return;

	if (IsGrabbing()) return;

	TOptional<FHitResult> HitResult = GetGrabbableInReach();
	if (HitResult.IsSet())
	{
		UPrimitiveComponent* HitComponent = HitResult.GetValue().GetComponent();

		if (!HitComponent) return;

		AActor* HitActor = HitResult->GetActor();
		HitActor->Tags.Add(grabbedTag);

		HitComponent->SetSimulatePhysics(true);
		HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		
		HitComponent->WakeAllRigidBodies();
		PhysicsHandle->GrabComponentAtLocationWithRotation(HitComponent, NAME_None, HitResult.GetValue().ImpactPoint, GetComponentRotation());
	}
}

void UGrabber::Release() 
{
	UE_LOG(LogTemp, Display, TEXT("Released."));

	if (!IsGrabbing()) return;

	UPrimitiveComponent* grabbedComponent = PhysicsHandle->GetGrabbedComponent();
	grabbedComponent->GetAttachmentRootActor()->Tags.Remove(grabbedTag);
	grabbedComponent->WakeAllRigidBodies();
	PhysicsHandle->ReleaseComponent();
}