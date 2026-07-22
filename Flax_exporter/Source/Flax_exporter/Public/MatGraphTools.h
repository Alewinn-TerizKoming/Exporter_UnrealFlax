#pragma once

#include "CoreMinimal.h"

class UMaterial;
class UMaterialInstance;

class FLAX_EXPORTER_API MatGraphTools
{
public:
	MatGraphTools();
	~MatGraphTools();

	static FString GetColor(UObject* Asset);

private:

    static UMaterial* GetParentMaterial(UMaterialInstance* MaterialInstance);
    static FString GenerateRandomColor();
};
