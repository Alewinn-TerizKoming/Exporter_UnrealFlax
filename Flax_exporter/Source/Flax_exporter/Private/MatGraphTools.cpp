
#include "MatGraphTools.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstance.h"

#include "UObject/Field.h"
#include "UObject/UnrealType.h"

MatGraphTools::MatGraphTools()
{
}

MatGraphTools::~MatGraphTools()
{
}

FString MatGraphTools::GetColor(UObject* Asset)
{
    if (!Asset)
    {
        UE_LOG(LogTemp, Warning, TEXT("Asset is null."));
        return FString();
    }

    if (UMaterialInstance* MI = Cast<UMaterialInstance>(Asset))
    {
        return GenerateRandomColor();
    }

    if (UMaterial* Mat = Cast<UMaterial>(Asset))
    {
        return GenerateRandomColor();
    }

    UE_LOG(LogTemp,Warning,TEXT("%s is not a Material."),*Asset->GetClass()->GetName());
    return FString();
}

UMaterial* MatGraphTools::GetParentMaterial(UMaterialInstance* MaterialInstance)
{
    if (!MaterialInstance) { return nullptr; }

    UMaterialInterface* Parent = MaterialInstance->Parent;

    if (!Parent) { return nullptr; }

    if (UMaterial* Material = Cast<UMaterial>(Parent)) { return Material; }

    if (UMaterialInstance* ParentInstance = Cast<UMaterialInstance>(Parent))
    {
        return GetParentMaterial(ParentInstance);
    }

    return nullptr;
}

FString MatGraphTools::GenerateRandomColor()
{
    const uint8 R = FMath::RandRange(0, 255);
    const uint8 G = FMath::RandRange(0, 255);
    const uint8 B = FMath::RandRange(0, 255);

    return FString::Printf(TEXT("#%02X%02X%02X%02X"),R,G,B,255);
}