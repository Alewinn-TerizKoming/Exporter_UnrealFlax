// Copyright Epic Games, Inc. All Rights Reserved.

#include "Flax_exporter.h"
#include "Flax_exporterStyle.h"
#include "Flax_exporterCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "Editor.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "EditorFramework/AssetImportData.h"
#include "DesktopPlatformModule.h"
#include "FlaxExportTypes.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "Dom/JsonObject.h"
#include "Misc/FileHelper.h"
#include "FAssetExporter.h"
#include "JsonObjectConverter.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstance.h"
#include "Materials/Material.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"

#include "MatGraphTools.h"
#include "Exporters/MeshExporter.h"

static const FName Flax_exporterTabName("Flax_exporter");

#define LOCTEXT_NAMESPACE "FFlax_exporterModule"

void FFlax_exporterModule::StartupModule()
{

	FFlax_exporterStyle::Initialize();
	FFlax_exporterStyle::ReloadTextures();

	FFlax_exporterCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FFlax_exporterCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FFlax_exporterModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(
		FSimpleMulticastDelegate::FDelegate::CreateRaw(
			this,
			&FFlax_exporterModule::RegisterMenus));
}

void FFlax_exporterModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
	FFlax_exporterStyle::Shutdown();
	FFlax_exporterCommands::Unregister();
}

void FFlax_exporterModule::PluginButtonClicked()
{
	UWorld* World = nullptr;

	if (GEditor)
	{
		World = GEditor->GetEditorWorldContext().World();
	}

	if (!World)
	{
		FMessageDialog::Open(
			EAppMsgType::Ok,
			FText::FromString("No editor world found"));

		return;
	}

	ExportScene(World);
}

void FFlax_exporterModule::ExportScene(UWorld* World)
{
	FFlaxExportScene Scene;

	Scene.SceneName = FPackageName::GetShortName(World->GetPackage()->GetName());
	Scene.From = TEXT("Unreal");

	for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		if (AStaticMeshActor* StaticMeshActor = Cast<AStaticMeshActor>(*ActorItr))
		{
			MeshExporter::Export(StaticMeshActor,Scene);
		}
	}

	const FString ExportFolder = ChooseExportFolder();

	if (ExportFolder.IsEmpty())
	{
		return;
	}

	SaveScene(Scene, ExportFolder);
	ExportStaticMeshes(Scene, ExportFolder);

	UE_LOG(LogTemp, Log, TEXT("Export scene completed."));
}

FString FFlax_exporterModule::ChooseExportFolder()
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();

	if (!DesktopPlatform)
	{
		return FString();
	}

	FString Folder;

	bool bOpened = DesktopPlatform->OpenDirectoryDialog(
		nullptr,
		TEXT("Choose export folder"),
		TEXT(""),
		Folder
	);

	if (bOpened)
	{
		return Folder;
	}

	return FString();
}

bool FFlax_exporterModule::SaveScene(const FFlaxExportScene& Scene, const FString& ExportFolder)
{
	const FString SceneFilename = ExportFolder / (Scene.SceneName + TEXT(".json"));

	return WriteSceneJson(Scene, SceneFilename);
}

void FFlax_exporterModule::ExportStaticMeshes(const FFlaxExportScene& Scene, const FString& ExportFolder)
{
	FAssetExporter AssetExporter;

	for (const FFlaxExportStaticMesh& MeshInfo : Scene.StaticMeshes)
	{
		UStaticMesh* Mesh = LoadObject<UStaticMesh>(
			nullptr,
			*MeshInfo.AssetPath
		);

		if (!Mesh)
		{
			UE_LOG(
				LogTemp,
				Warning,
				TEXT("Unable to load mesh %s"),
				*MeshInfo.AssetPath
			);

			continue;
		}

		FString Message;

		const FString Filename = ExportFolder / (MeshInfo.Name + TEXT(".fbx"));

		if (!AssetExporter.Export(Mesh, Filename, Message))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("%s"), *Message);
		}
	}
}

bool FFlax_exporterModule::WriteSceneJson(const FFlaxExportScene& Scene, const FString& Filename)
{
	FString Json;

	if (!FJsonObjectConverter::UStructToJsonObjectString(Scene, Json))
	{
		return false;
	}

	const bool bSaved = FFileHelper::SaveStringToFile(Json, *Filename);

	if (bSaved)
	{
		UE_LOG(LogTemp, Log, TEXT("Scene written to %s"), *Filename);
	}

	return bSaved;
}

void FFlax_exporterModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
		Section.AddMenuEntryWithCommandList(FFlax_exporterCommands::Get().PluginAction, PluginCommands);
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");

		FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FFlax_exporterCommands::Get().PluginAction));

		Entry.SetCommandList(PluginCommands);
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FFlax_exporterModule, Flax_exporter)