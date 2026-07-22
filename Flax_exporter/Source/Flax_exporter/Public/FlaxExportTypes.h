// (c)GSH Generated code - do not modify, or all changes will be lost.

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectConverter.h"

#include "FlaxExportTypes.generated.h"

USTRUCT()
struct FLAX_EXPORTER_API FFlaxExportVector3
{
    GENERATED_BODY()

    UPROPERTY()
        float X;
    UPROPERTY()
        float Y;
    UPROPERTY()
        float Z;
};

USTRUCT()
struct FLAX_EXPORTER_API FFlaxExportRotation
{
    GENERATED_BODY()

    UPROPERTY()
        float Pitch;
    UPROPERTY()
        float Yaw;
    UPROPERTY()
        float Roll;
};

USTRUCT()
struct FLAX_EXPORTER_API FFlaxExportActorProperties
{
    GENERATED_BODY()

    UPROPERTY()
        bool Visibility;
    UPROPERTY()
        bool HiddenInGame;
    UPROPERTY()
        FString Mobility;
    UPROPERTY()
        bool CastShadow;
    UPROPERTY()
        TArray<FString> Tags;
    UPROPERTY()
        FString Layer;
};

USTRUCT()
struct FLAX_EXPORTER_API FFlaxExportStaticMesh
{
    GENERATED_BODY()

    UPROPERTY()
        FString AssetPath;
    UPROPERTY()
        FString Name;

    bool operator==(const FFlaxExportStaticMesh& Other) const
    {
        return AssetPath == Other.AssetPath;
    }
};

USTRUCT()
struct FLAX_EXPORTER_API FFlaxExportMaterialSlot
{
    GENERATED_BODY()

    UPROPERTY()
        FString AssetPath;
    UPROPERTY()
        FString Name;
    UPROPERTY()
        FString ParentMaterial;
    UPROPERTY()
        FString Color;

    bool operator==(const FFlaxExportMaterialSlot& Other) const
    {
        return AssetPath == Other.AssetPath;
    }
};

USTRUCT()
struct FLAX_EXPORTER_API FFlaxExportMeshInstance
{
    GENERATED_BODY()

    UPROPERTY()
        FString Name;
    UPROPERTY()
        FString Folder;
    UPROPERTY()
        int32 Mesh;
    UPROPERTY()
        FVector Location;
    UPROPERTY()
        FRotator Rotation;
    UPROPERTY()
        FVector Scale;
    UPROPERTY()
        FFlaxExportActorProperties Properties;
    UPROPERTY()
        TArray<int32> Materials;
};

USTRUCT()
struct FLAX_EXPORTER_API FFlaxExportScene
{
    GENERATED_BODY()

    UPROPERTY()
        FString SceneName;
    UPROPERTY()
        TArray<FFlaxExportStaticMesh> StaticMeshes;
    UPROPERTY()
        TArray<FFlaxExportMeshInstance> MeshInstances;
    UPROPERTY()
        TArray<FFlaxExportMaterialSlot> Materials;
};

