#include "FAssetExporter.h"
#include "UObject/GCObjectScopeGuard.h"
#include "AssetExportTask.h"
#include "Exporters/Exporter.h"
#include "Misc/Paths.h"
#include "Exporters/FbxExportOption.h"

FAssetExporter::FAssetExporter()
{
}

FAssetExporter::~FAssetExporter()
{
}

bool FAssetExporter::Export(UObject* Asset, const FString DestinationPath, FString& OutInfoMessage)
{
	FString Extension = FPaths::GetExtension(DestinationPath).ToLower();
	UExporter* Exporter = UExporter::FindExporter(Asset, *Extension);

	if (Exporter == nullptr)
	{
		OutInfoMessage = FString::Printf(TEXT("Export asset failed - No exporter found for extension '%s'"), *Extension);
		return false;
	}

	UAssetExportTask* ExportTask = NewObject<UAssetExportTask>();
	FGCObjectScopeGuard ExportTaskGuard(ExportTask);

	Exporter->ExportTask = ExportTask;
	ExportTask->Exporter = Exporter;

	ExportTask->Object = Asset;
	ExportTask->Filename = DestinationPath;
	ExportTask->bReplaceIdentical = true;
	ExportTask->bPrompt = false;
	ExportTask->bAutomated = true;
	ExportTask->bUseFileArchive = false;
	ExportTask->bWriteEmptyFiles = false;
	ExportTask->bSelected = false;
	ExportTask->IgnoreObjectList = TArray<TObjectPtr<UObject>>();

	if (Extension == "fbx")
	{
		UFbxExportOption* Options = NewObject<UFbxExportOption>();
		Options->FbxExportCompatibility = EFbxExportCompatibility::FBX_2013;
		Options->bASCII = false;
		Options->bForceFrontXAxis = false;
		Options->VertexColor = true;
		Options->LevelOfDetail = true;
		Options->Collision = true;
		Options->bExportMorphTargets = true;
		Options->bExportPreviewMesh = false;
		Options->MapSkeletalMotionToRoot = false;
		Options->bExportLocalTime = true;

		ExportTask->Options = Options;
	}

	bool ExportSuccess = UExporter::RunAssetExportTask(ExportTask);

	if (!ExportSuccess)
	{
		OutInfoMessage = FString::Printf(TEXT("Export asset failed - Error: '%s'"), *FString::Join(ExportTask->Errors, TEXT(", ")));
		return false;
	}

	OutInfoMessage = FString::Printf(TEXT("Export asset succeeded - '%s'"), *DestinationPath);
    return true;
}