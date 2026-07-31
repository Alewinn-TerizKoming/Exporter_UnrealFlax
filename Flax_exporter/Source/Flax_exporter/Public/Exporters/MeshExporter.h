// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class AStaticMeshActor;
struct FFlaxExportScene;

/**
 * 
 */
class MeshExporter
{
public:
	static void Export(AStaticMeshActor* Actor, FFlaxExportScene& Scene);
};
