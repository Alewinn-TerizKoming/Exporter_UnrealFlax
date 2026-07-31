#pragma once

#include "CoreMinimal.h"
#include "JsonObjectConverter.h"

#include "FlaxExportTypes.generated.h"

UENUM()
enum class FFlaxExportLightType : uint8
{
    Directional,
    Point,
    Spot,
    Sky,
};

USTRUCT()
struct FLAX_EXPORTER_API FFlaxExportVector3
{
    GENERATED_BODY()

    UPROPERTY()
        float X = 0.0f;
    UPROPERTY()
        float Y = 0.0f;
    UPROPERTY()
        float Z = 0.0f;
};

USTRUCT()
struct FLAX_EXPORTER_API FFlaxExportRotation
{
    GENERATED_BODY()

    UPROPERTY()
        float Pitch = 0.0f;
    UPROPERTY()
        float Yaw = 0.0f;
    UPROPERTY()
        float Roll = 0.0f;
};

USTRUCT()
struct FLAX_EXPORTER_API FFlaxExportActorProperties
{
    GENERATED_BODY()

    UPROPERTY()
        bool Visibility = false;
    UPROPERTY()
        bool HiddenInGame = false;
    UPROPERTY()
        FString Mobility = "";
    UPROPERTY()
        bool CastShadow = false;
    UPROPERTY()
        TArray<FString> Tags ;
    UPROPERTY()
        FString Layer = "";
};

USTRUCT()
struct FLAX_EXPORTER_API FFlaxExportLightSettings
{
    GENERATED_BODY()

    UPROPERTY()
        FString Color = "";
    UPROPERTY()
        float Brightness = 0.0f;
    UPROPERTY()
        float ViewDistance = 0.0f;
    UPROPERTY()
        float MinimumRoughness = 0.0f;
    UPROPERTY()
        float IndirectLightingIntensity = 0.0f;
};

USTRUCT()
struct FLAX_EXPORTER_API FFlaxExportShadowSettings
{
    GENERATED_BODY()

    UPROPERTY()
        FString Mode = "";
    UPROPERTY()
        FString PartitionMode = "";
    UPROPERTY()
        int32 CascadeCount = 0;
    UPROPERTY()
        float CascadeSpacing = 0.0f;
    UPROPERTY()
        float Sharpness = 0.0f;
    UPROPERTY()
        float Strength = 0.0f;
    UPROPERTY()
        float Distance = 0.0f;
    UPROPERTY()
        float FadeDistance = 0.0f;
    UPROPERTY()
        float DepthBias = 0.0f;
    UPROPERTY()
        float NormalOffsetScale = 0.0f;
    UPROPERTY()
        float ContactShadowLength = 0.0f;
    UPROPERTY()
        float Resolution = 0.0f;
};

USTRUCT()
struct FLAX_EXPORTER_API FFlaxExportVolumetricFogSettings
{
    GENERATED_BODY()

    UPROPERTY()
        float ScatteringIntensity = 0.0f;
    UPROPERTY()
        bool CastShadow = false;
};

USTRUCT()
struct FLAX_EXPORTER_API FFlaxExportLocalLightSettings
{
    GENERATED_BODY()

    UPROPERTY()
        float Radius = 0.0f;
    UPROPERTY()
        float SourceRadius = 0.0f;
    UPROPERTY()
        float SourceLength = 0.0f;
    UPROPERTY()
        bool UseInverseSquaredFalloff = false;
    UPROPERTY()
        float FallOffExponent = 0.0f;
};

USTRUCT()
struct FLAX_EXPORTER_API FFlaxExportSpotLightSettings
{
    GENERATED_BODY()

    UPROPERTY()
        float InnerConeAngle = 0.0f;
    UPROPERTY()
        float OuterConeAngle = 0.0f;
};

USTRUCT()
struct FLAX_EXPORTER_API FFlaxExportSkyLightSettings
{
    GENERATED_BODY()

    UPROPERTY()
        FString AdditiveColor = "";
    UPROPERTY()
        FString Mode = "";
    UPROPERTY()
        float SkyDistanceThreshold = 0.0f;
    UPROPERTY()
        FString CustomTexture = "";
};

USTRUCT()
struct FLAX_EXPORTER_API FFlaxExportIESSettings
{
    GENERATED_BODY()

    UPROPERTY()
        FString Texture = "";
    UPROPERTY()
        bool UseBrightness = false;
    UPROPERTY()
        float BrightnessScale = 0.0f;
};

USTRUCT()
struct FLAX_EXPORTER_API FFlaxExportLight
{
    GENERATED_BODY()

    UPROPERTY()
        FString Name = "";
    UPROPERTY()
        FString Folder = "";
    UPROPERTY()
        FFlaxExportLightType Type ;
    UPROPERTY()
        FVector Location = FVector::ZeroVector;
    UPROPERTY()
        FRotator Rotation = FRotator::ZeroRotator;
    UPROPERTY()
        FVector Scale = FVector::ZeroVector;
    UPROPERTY()
        FFlaxExportActorProperties Properties ;
    UPROPERTY()
        FFlaxExportLightSettings Settings ;
    UPROPERTY()
        FFlaxExportShadowSettings Shadow ;
    UPROPERTY()
        FFlaxExportVolumetricFogSettings VolumetricFog ;
    UPROPERTY()
        FFlaxExportLocalLightSettings Local ;
    UPROPERTY()
        FFlaxExportSpotLightSettings Spot ;
    UPROPERTY()
        FFlaxExportSkyLightSettings Sky ;
    UPROPERTY()
        FFlaxExportIESSettings IES ;
};

USTRUCT()
struct FLAX_EXPORTER_API FFlaxExportStaticMesh
{
    GENERATED_BODY()

    UPROPERTY()
        FString AssetPath = "";
    UPROPERTY()
        FString Name = "";

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
        FString AssetPath = "";
    UPROPERTY()
        FString Name = "";
    UPROPERTY()
        FString ParentMaterial = "";
    UPROPERTY()
        FString Color = "";

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
        FString Name = "";
    UPROPERTY()
        FString Folder = "";
    UPROPERTY()
        int32 Mesh = 0;
    UPROPERTY()
        FVector Location = FVector::ZeroVector;
    UPROPERTY()
        FRotator Rotation = FRotator::ZeroRotator;
    UPROPERTY()
        FVector Scale = FVector::ZeroVector;
    UPROPERTY()
        FFlaxExportActorProperties Properties ;
    UPROPERTY()
        TArray<int32> Materials ;
};

USTRUCT()
struct FLAX_EXPORTER_API FFlaxExportScene
{
    GENERATED_BODY()

    UPROPERTY()
        FString SceneName = "";
    UPROPERTY()
        TArray<FFlaxExportStaticMesh> StaticMeshes ;
    UPROPERTY()
        TArray<FFlaxExportMeshInstance> MeshInstances ;
    UPROPERTY()
        TArray<FFlaxExportLight> Lights ;
    UPROPERTY()
        TArray<FFlaxExportMaterialSlot> Materials ;
};

