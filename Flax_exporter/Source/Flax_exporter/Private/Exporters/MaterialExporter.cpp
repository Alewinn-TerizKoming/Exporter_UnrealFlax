// Fill out your copyright notice in the Description page of Project Settings.


#include "Exporters/MaterialExporter.h"
#include "FlaxExportTypes.h"
#include "MatGraphTools.h"

int32 MaterialExporter::RegisterMaterial(UMaterialInterface* Material, FFlaxExportScene& Scene)
{
	if (!Material) { return -1; }

	FFlaxExportMaterialSlot Slot;

	Slot.AssetPath = Material->GetPathName();
	Slot.Name = Material->GetName();
	Slot.Color = MatGraphTools::GetColor(Material);

	UMaterialInstance* Instance = Cast<UMaterialInstance>(Material);

	if (Instance && Instance->Parent)
	{
		Slot.ParentMaterial = Instance->Parent->GetPathName();

		// Add parent to the list
		RegisterMaterial(Instance->Parent, Scene);
	}

	return Scene.Materials.AddUnique(Slot);
}