#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class FLAX_EXPORTER_API AssetExporter
{
public:
	AssetExporter();
	~AssetExporter();

	bool Export(UObject* Asset, const FString& DestinationPath);
};
