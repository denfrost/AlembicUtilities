// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeometryCacheActor.h"
#include "GeometryCacheComponent.h"

#include "GeometryCache.h"
#include "GeometryCacheTrackStreamable.h"
#include "GeometryCacheMeshData.h"

#include "AlembicSocketActor.generated.h"

UCLASS()
class GALEMBICUTILITIES_API AAlembicSocketActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAlembicSocketActor();

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Alembic")
		AGeometryCacheActor* GeometryCacheActor;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Alembic")
		float DistanceThreshold;

	void SetInitialVertices(float Time);

	FVector GetVertexCenter(float Time);
	void GetActorMinMaxBounds(FVector& Origin, FVector& MinBounds, FVector& MaxBounds);

	UPROPERTY()
		TArray<int32> InfluentialVertexIDs;

	UPROPERTY()
		FVector InitialDisplacement;

	UPROPERTY()
		float CurrentTime;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
