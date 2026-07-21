#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class FLAX_EXPORTER_API FAssetExporter
{
public:
	FAssetExporter();
	~FAssetExporter();

	bool Export(UObject* Asset,const FString DestinationPath, FString& OutInfoMessage);
};