// Fill out your copyright notice in the Description page of Project Settings.

#include "Exporters/ActorExporter.h"
#include "Engine/StaticMeshActor.h"
#include "FlaxExportTypes.h"

FFlaxExportActorProperties ActorExporter::ExtractActorProperties(AStaticMeshActor* Actor, UStaticMeshComponent* MeshComponent)
{
	FFlaxExportActorProperties Properties;

	// Visibility
	Properties.Visibility = MeshComponent->IsVisible();

	// Hidden in game
	Properties.HiddenInGame = Actor->IsHidden() || MeshComponent->bHiddenInGame;

	// Mobility
	switch (MeshComponent->Mobility)
	{
	case EComponentMobility::Static:
		Properties.Mobility = TEXT("Static");
		break;

	case EComponentMobility::Stationary:
		Properties.Mobility = TEXT("Stationary");
		break;

	case EComponentMobility::Movable:
		Properties.Mobility = TEXT("Movable");
		break;

	default:
		Properties.Mobility = TEXT("");
		break;
	}

	// Shadows
	Properties.CastShadow = MeshComponent->CastShadow;

	// Tags
	for (const FName& Tag : Actor->Tags)
	{
		Properties.Tags.Add(Tag.ToString());
	}

	// Layer
	// Not handled yet
	Properties.Layer = TEXT("");

	return Properties;
}