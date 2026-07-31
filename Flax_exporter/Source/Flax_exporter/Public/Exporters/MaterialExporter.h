// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Materials/Material.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInterface.h"
#include "CoreMinimal.h"

struct FFlaxExportScene;

/**
 * 
 */
class MaterialExporter
{
public:
	int32 static RegisterMaterial(UMaterialInterface* Material, FFlaxExportScene& Scene);
};
