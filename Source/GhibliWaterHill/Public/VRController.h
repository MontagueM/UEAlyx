// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VRController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFlingEvent, USplineComponent*, FlickPath, UPrimitiveComponent*, FlickedComponent);

UCLASS()
class GHIBLIWATERHILL_API AVRController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVRController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetHand(EControllerHand Hand);
	bool bCanHandTeleport();
	bool bCanHandMove();
	bool FindTeleportDestination(FVector& Location);
	bool UpdateTeleportationCheck();
	void SetCanCheckTeleport(bool bCheck);
	void TryGrab();
	void ReleaseGrab();
	void DetectGrabStyle();

	bool bAllowCharacterTeleport = false;

	UPROPERTY(EditDefaultsOnly)
	EControllerHand TeleportHand = EControllerHand::Left;
	EControllerHand Hand;
private:
	// 	UPROPERTY(VisibleAnywhere)

	UPROPERTY(VisibleAnywhere)
	class UMotionControllerComponent* MotionController;

	UPROPERTY(EditDefaultsOnly)
	class UCurveFloat* FlickAngleCurve = nullptr;
private:
	UPROPERTY()
	class USplineMeshComponent* SplineMesh = nullptr;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* DestinationMarker = nullptr;
	UPROPERTY(VisibleAnywhere)
	class USplineComponent* TeleportPath = nullptr;
	UPROPERTY(VisibleAnywhere)
	class USplineComponent* FlickPath = nullptr;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* MarkerPoint = nullptr;
	UPROPERTY(VisibleAnywhere)
	class UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* GrabVolume = nullptr;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* ControllerMesh = nullptr;
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* FlickRoot = nullptr;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* FlickVolume = nullptr;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* DebugMesh = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* LeftControllerMesh = nullptr;
	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* RightControllerMesh = nullptr;

	UPROPERTY(EditDefaultsOnly)
	float TeleportProjectileSpeed = 800;
	UPROPERTY(EditDefaultsOnly)
	float TeleportProjectileRadius = 10;
	UPROPERTY(EditDefaultsOnly)
	float TeleportSimulationTime = 5;
	UPROPERTY(EditDefaultsOnly)
	float TeleportSimulationFrequency = 50;
	UPROPERTY(EditDefaultsOnly)
	FVector TeleportNavExtent = FVector(100, 100, 100);
	UPROPERTY(EditDefaultsOnly)
	class UStaticMesh* TeleportArcMesh;
	UPROPERTY(EditDefaultsOnly)
	class UMaterialInterface* TeleportArcMaterial;
	UPROPERTY() // need this for proper garbage collection
	TArray<class USplineMeshComponent*> MeshObjects;
	UPROPERTY(EditDefaultsOnly)
	FVector DestinationMarkerScale = FVector(0.7, 0.7, 0.5);


	bool bCanCheckTeleport = false;
	bool bIsGrabbing = false;
	class UPrimitiveComponent* GrabbedComponent = nullptr;
	float GrabbedComponentInitDistance;
	FRotator ControllerRotationOnGrab;
private:
	void UpdateSpline(TArray<FVector> PathData, USplineComponent* PathToUpdate);
	bool ProjectilePathingUpdate(struct FPredictProjectilePathResult& Result, float ProjectileRadius, FVector StartLocation, FVector Direction, float ProjectileSpeed, float SimulationTime, ECollisionChannel CollisionChannel);

private:
	void FlickHighlight();
	void UpdateFlickSpline();
	void TryFlick();
	bool bUpVelocityForFlick();
	void ReleaseFlick();
	UFUNCTION(BlueprintCallable)
	void ResetRegisteredComponents();
	void ModifySplinePoints(USplineComponent* PathToUpdate, bool bHidePoints, bool bClear);
	TArray<FVector> PathPointDataToFVector(TArray<struct FPredictProjectilePathPointData> PathData);
	
	UPrimitiveComponent* RegisteredFlickComponent = nullptr;
	USplineComponent* RegisteredSplineComponent = nullptr;
	FVector RegisteredControllerLocation = FVector::ZeroVector;
	UPrimitiveComponent* ComponentCurrentlyFlicking = nullptr;
	bool bHoldingFlick = false;
	bool bOnOldComponent = true;
	float FlickVelocityRequired = 250;

public:
	UPROPERTY(BlueprintAssignable)
	FFlingEvent StartComponentFling;

	void DetectReleaseStyle();
	bool bGoodFlickRotation();
};
