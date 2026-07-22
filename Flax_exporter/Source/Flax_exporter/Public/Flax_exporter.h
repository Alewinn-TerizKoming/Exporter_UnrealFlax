// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Materials/Material.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInterface.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;
class UWorld;
class UStaticMesh;
struct FFlaxExportScene;
class AStaticMeshActor;
class UStaticMeshComponent;
struct FFlaxExportActorProperties;
struct FFlaxExportMeshInstance;
struct FFlaxExportMaterialSlot;

class FFlax_exporterModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/** Commands. */
	void PluginButtonClicked();

private:


	void RegisterMenus();
	FString ChooseExportFolder();

	void ExportScene(UWorld* World);
	void ExportStaticMeshes(const FFlaxExportScene& Scene,const FString& ExportFolder);
	bool WriteSceneJson(const FFlaxExportScene& Scene, const FString& Filename);
	bool SaveScene(const FFlaxExportScene& Scene,const FString& ExportFolder);

private:
	int32 RegisterMaterial(UMaterialInterface* Material, FFlaxExportScene& Scene);
	FFlaxExportActorProperties ExtractActorProperties(AStaticMeshActor* Actor,UStaticMeshComponent* MeshComponent);
	TSharedPtr<class FUICommandList> PluginCommands;
};