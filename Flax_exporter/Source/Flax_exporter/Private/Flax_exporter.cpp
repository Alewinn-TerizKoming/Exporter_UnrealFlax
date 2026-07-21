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

	//PluginCommands->MapAction(
	//	FFlax_exporterCommands::Get().GenerateMaterialColor,
	//	FExecuteAction::CreateRaw(this,&FFlax_exporterModule::GenerateMaterialColorButtonClicked),
	//	FCanExecuteAction());

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

//void FFlax_exporterModule::GenerateMaterialColorButtonClicked()
//{
//	FContentBrowserModule& ContentBrowserModule =
//		FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
//
//	TArray<FAssetData> SelectedAssets;
//	ContentBrowserModule.Get().GetSelectedAssets(SelectedAssets);
//
//	if (SelectedAssets.Num() == 0)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("No asset selected."));
//		return;
//	}
//
//	UObject* Asset = SelectedAssets[0].GetAsset();
//
//	if (!Asset)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Unable to load selected asset."));
//		return;
//	}
//
//	FString Color = MatGraphTools::GetColor(Asset);
//
//	UE_LOG(LogTemp, Log, TEXT("Color : %s"), *Color);
//}

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

void FFlax_exporterModule::ExportScene(UWorld* World)
{
	FFlaxExportScene Scene;

	Scene.SceneName = FPackageName::GetShortName(World->GetPackage()->GetName());

	for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		AStaticMeshActor* StaticMeshActor = Cast<AStaticMeshActor>(*ActorItr);

		if (!StaticMeshActor)
		{
			continue;
		}

		UStaticMeshComponent* MeshComponent = StaticMeshActor->GetStaticMeshComponent();

		if (!MeshComponent)
		{
			continue;
		}

		UStaticMesh* Mesh = MeshComponent->GetStaticMesh();

		if (!Mesh)
		{
			continue;
		}

		FFlaxExportStaticMesh StaticMesh;

		StaticMesh.AssetPath = Mesh->GetPathName();
		StaticMesh.Name = Mesh->GetName();

		int32 MeshIndex = Scene.StaticMeshes.AddUnique(StaticMesh);

		FFlaxExportMeshInstance Instance;

		Instance.Name = StaticMeshActor->GetActorLabel();
		Instance.Folder = StaticMeshActor->GetFolderPath().ToString();

		if (Instance.Name.IsEmpty())
		{
			Instance.Name = StaticMeshActor->GetName();
		}

		Instance.Mesh = MeshIndex;

		FTransform Transform = StaticMeshActor->GetActorTransform();

		Instance.Location = Transform.GetLocation();
		Instance.Rotation = Transform.GetRotation().Rotator();
		Instance.Scale = Transform.GetScale3D();
		Instance.Properties = ExtractActorProperties(StaticMeshActor, MeshComponent);
		
		for (int32 i = 0; i < MeshComponent->GetNumMaterials(); i++)
		{
			//UMaterialInterface* Material = MeshComponent->GetMaterial(i);

			UMaterialInterface* Material = nullptr;

			if (MeshComponent->OverrideMaterials.IsValidIndex(i) &&
				MeshComponent->OverrideMaterials[i])
			{
				Material = MeshComponent->OverrideMaterials[i];
			}
			else
			{
				Material = MeshComponent->GetMaterial(i);
			}

			int32 MaterialIndex = RegisterMaterial(Material, Scene);
			if (MaterialIndex >= 0) { Instance.Materials.Add(MaterialIndex); }

		}

		UE_LOG(
			LogTemp,
			Log,
			TEXT("Mesh %s uses %d materials"),
			*Mesh->GetName(),
			Instance.Materials.Num());

		Scene.MeshInstances.Add(Instance);
	}

	UE_LOG(
		LogTemp,
		Log,
		TEXT("Unique meshes : %d"),
		Scene.StaticMeshes.Num()
	);

	UE_LOG(
		LogTemp,
		Log,
		TEXT("Export scene contains %d meshes"),
		Scene.MeshInstances.Num()
	);

	const FString ExportFolder = ChooseExportFolder();

	if (ExportFolder.IsEmpty())
	{
		return;
	}

	SaveScene(Scene, ExportFolder);
	ExportStaticMeshes(Scene, ExportFolder);
}

FFlaxExportActorProperties FFlax_exporterModule::ExtractActorProperties(AStaticMeshActor* Actor, UStaticMeshComponent* MeshComponent)
{
	FFlaxExportActorProperties Properties;

	// Visibility
	Properties.Visibility = MeshComponent->IsVisible();

	// Hidden in game
	Properties.HiddenInGame = Actor->IsHidden() || MeshComponent->bHiddenInGame;

	// Mobility
	switch (MeshComponent->Mobility)
	{
	case EComponentMobility::Static:
		Properties.Mobility = TEXT("Static");
		break;

	case EComponentMobility::Stationary:
		Properties.Mobility = TEXT("Stationary");
		break;

	case EComponentMobility::Movable:
		Properties.Mobility = TEXT("Movable");
		break;

	default:
		Properties.Mobility = TEXT("");
		break;
	}


	// Shadows
	Properties.CastShadow = MeshComponent->CastShadow;


	// Tags
	for (const FName& Tag : Actor->Tags)
	{
		Properties.Tags.Add(Tag.ToString());
	}


	// Layer
	// Pas encore géré
	Properties.Layer = TEXT("");


	return Properties;
}

int32 FFlax_exporterModule::RegisterMaterial(UMaterialInterface* Material,FFlaxExportScene& Scene)
{
	if (!Material){ return -1; }

	FFlaxExportMaterialSlot Slot;

	Slot.AssetPath = Material->GetPathName();
	Slot.Name = Material->GetName();
	Slot.Color = MatGraphTools::GetColor(Material);

	UMaterialInstance* Instance = Cast<UMaterialInstance>(Material);

	if (Instance && Instance->Parent)
	{
		Slot.ParentMaterial = Instance->Parent->GetPathName();

		// Ajouter le parent dans la liste
		RegisterMaterial( Instance->Parent, Scene);
	}

	return Scene.Materials.AddUnique(Slot);
}

bool FFlax_exporterModule::SaveScene(const FFlaxExportScene& Scene, const FString& ExportFolder)
{
	const FString SceneFilename =
		ExportFolder / (Scene.SceneName + TEXT(".json"));

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

bool FFlax_exporterModule::WriteSceneJson(
	const FFlaxExportScene& Scene,
	const FString& Filename)
{
	FString Json;

	if (!FJsonObjectConverter::UStructToJsonObjectString(Scene, Json))
	{
		return false;
	}

	const bool bSaved = FFileHelper::SaveStringToFile(Json, *Filename);

	if (bSaved)
	{
		UE_LOG(
			LogTemp,
			Log,
			TEXT("Scene written to %s"),
			*Filename);
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
		//Section.AddMenuEntryWithCommandList(FFlax_exporterCommands::Get().GenerateMaterialColor, PluginCommands);
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");

		FToolMenuEntry& Entry =
			Section.AddEntry(
				FToolMenuEntry::InitToolBarButton(
					FFlax_exporterCommands::Get().PluginAction));

		//FToolMenuEntry& MaterialEntry =
		//	Section.AddEntry(
		//		FToolMenuEntry::InitToolBarButton(
		//			FFlax_exporterCommands::Get().GenerateMaterialColor));

		Entry.SetCommandList(PluginCommands);
		//MaterialEntry.SetCommandList(PluginCommands);
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FFlax_exporterModule, Flax_exporter)