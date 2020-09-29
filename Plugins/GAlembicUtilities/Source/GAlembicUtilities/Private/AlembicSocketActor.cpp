// Fill out your copyright notice in the Description page of Project Settings.


#include "AlembicSocketActor.h"

// Sets default values
AAlembicSocketActor::AAlembicSocketActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DistanceThreshold = 10;
}

// Called when the game starts or when spawned
void AAlembicSocketActor::BeginPlay()
{
	Super::BeginPlay();
	
	SetInitialVertices(0);

	FVector Center = GetVertexCenter(0);
	InitialDisplacement = GetActorLocation() - Center;
}

void AAlembicSocketActor::GetActorMinMaxBounds(FVector& Origin, FVector& MinBounds, FVector& MaxBounds)
{
	FBox Box(ForceInit);

	for (const UActorComponent* ActorComponent : GetComponents())
	{
		const UPrimitiveComponent* PrimComp = Cast<const UPrimitiveComponent>(ActorComponent);
		if (PrimComp)
		{
			Box += PrimComp->Bounds.GetBox();
		}
	}

	Origin = Box.GetCenter();
	MinBounds = Origin - Box.GetExtent();
	MaxBounds = MinBounds + Box.GetExtent() / 0.5f;

}


void AAlembicSocketActor::SetInitialVertices(float Time)
{
	FVector Origin, MinBounds, MaxBounds;
	GetActorMinMaxBounds(Origin, MinBounds, MaxBounds);

	if (GeometryCacheActor)
	{
		InfluentialVertexIDs.Empty();
		UGeometryCacheTrackStreamable* CurrentTrack = Cast<UGeometryCacheTrackStreamable>(GeometryCacheActor->GetGeometryCacheComponent()->GetGeometryCache()->Tracks[0]);
		if (CurrentTrack)
		{
			FTransform CacheTransform = GeometryCacheActor->GetActorTransform();

			FGeometryCacheMeshData MeshData = CurrentTrack->GetMeshSample(Time);
			TArray<FVector> Vertices = MeshData.Positions;

			TArray<FVector> InfluencingVertices;
			for (int32 vId = 0; vId < Vertices.Num(); vId++)
			{
				FVector NewVertex = CacheTransform.TransformPosition(Vertices[vId]);

				if (MinBounds.X <= NewVertex.X && NewVertex.X <= MaxBounds.X)
				{
					if (MinBounds.Y <= NewVertex.Y && NewVertex.Y <= MaxBounds.Y)
					{
						if (MinBounds.Z <= NewVertex.Z && NewVertex.Z <= MaxBounds.Z)
						{
							InfluentialVertexIDs.Add(vId);
						}
					}
				}
			}

			//GEngine->AddOnScreenDebugMessage(1, 10, FColor::Red, FString::FromInt(InfluencingVertices.Num()));

		}
	}

}

FVector AAlembicSocketActor::GetVertexCenter(float Time)
{
	FVector Origin, MinBounds, MaxBounds;
	GetActorMinMaxBounds(Origin, MinBounds, MaxBounds);

	if (GeometryCacheActor)
	{

		UGeometryCacheTrackStreamable* CurrentTrack = Cast<UGeometryCacheTrackStreamable>(GeometryCacheActor->GetGeometryCacheComponent()->GetGeometryCache()->Tracks[0]);
		if (CurrentTrack)
		{
			FTransform CacheTransform = GeometryCacheActor->GetActorTransform();

			FGeometryCacheMeshData MeshData = CurrentTrack->GetMeshSample(Time);
			TArray<FVector> Vertices = MeshData.Positions;

			TArray<FVector> InfluencingVertices;
			FBox BoundingBox;

			for (auto vId : InfluentialVertexIDs)
			{
				if (Vertices.Num() > vId)
				{
					FVector NewVertex = CacheTransform.TransformPosition(Vertices[vId]);
					InfluencingVertices.Add(NewVertex);
					BoundingBox.Min = Vertices[vId];
					BoundingBox.Max = Vertices[vId];

				}

			}

			if (InfluencingVertices.Num() > 0)
			{
				for (auto vt : InfluencingVertices)
				{
					if (vt.X < BoundingBox.Min.X)
					{
						BoundingBox.Min.X = vt.X;
					}
					if (vt.Y < BoundingBox.Min.Y)
					{
						BoundingBox.Min.Y = vt.Y;
					}
					if (vt.Z < BoundingBox.Min.Z)
					{
						BoundingBox.Min.Z = vt.Z;
					}
					if (vt.X > BoundingBox.Max.X)
					{
						BoundingBox.Max.X = vt.X;
					}
					if (vt.Y > BoundingBox.Max.Y)
					{
						BoundingBox.Max.Y = vt.Y;
					}
					if (vt.Z > BoundingBox.Max.Z)
					{
						BoundingBox.Max.Z = vt.Z;
					}
				}

				FVector Center = BoundingBox.GetCenter();
				return Center;
			}

		}
	}

	return GetActorLocation();
}

// Called every frame
void AAlembicSocketActor::Tick(float DeltaTime)
{
	CurrentTime += DeltaTime;
	Super::Tick(DeltaTime);
	FVector Center = GetVertexCenter(CurrentTime);

	SetActorLocation(InitialDisplacement + Center);

}

