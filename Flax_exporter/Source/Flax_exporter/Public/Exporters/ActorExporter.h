// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class AStaticMeshActor;
class UStaticMeshComponent;
struct FFlaxExportActorProperties;

/**
 * 
 */
class ActorExporter
{
public:
	FFlaxExportActorProperties static ExtractActorProperties(AStaticMeshActor* Actor, UStaticMeshComponent* MeshComponent);
};
