#pragma once

#include "CoreMinimal.h"

class UMaterial;
class UMaterialInstance;

/**
 * 
 */
class FLAX_EXPORTER_API MatGraphTools
{
public:
	MatGraphTools();
	~MatGraphTools();

	static FString GetColor(UObject* Asset);

private:

    static FString DumpMatColor(UMaterial* Material);

    static FString DumpMatInstColor(UMaterialInstance* MaterialInstance);

    static UMaterial* GetParentMaterial(UMaterialInstance* MaterialInstance);

    static void DumpProperties(UObject* Object);
    static FString GenerateRandomColor();
};
