
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
        //return DumpMatInstColor(MI);
        return GenerateRandomColor();
    }

    if (UMaterial* Mat = Cast<UMaterial>(Asset))
    {
        //return DumpMatColor(Mat);
        return GenerateRandomColor();
    }

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("%s is not a Material."),
        *Asset->GetClass()->GetName());

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

    return FString::Printf(
        TEXT("#%02X%02X%02X%02X"),
        R,
        G,
        B,
        255);
}

FString MatGraphTools::DumpMatInstColor(UMaterialInstance* MaterialInstance)
{
    UMaterial* Parent = GetParentMaterial(MaterialInstance);

    if (Parent)
    {
        UE_LOG(LogTemp,Log,
            TEXT("Parent Material : %s"),
            *Parent->GetName());
    }

    UE_LOG(LogTemp,Log,
        TEXT("Material Instance : %s"),
        *MaterialInstance->GetName());

    DumpProperties(MaterialInstance);

    return GenerateRandomColor();
}

FString MatGraphTools::DumpMatColor(
    UMaterial* Material)
{
    UE_LOG(LogTemp,Log,
        TEXT("Material : %s"),
        *Material->GetName());

    DumpProperties(Material);

    return GenerateRandomColor();
}

void MatGraphTools::DumpProperties(UObject* Object)
{
    UE_LOG(LogTemp, Log, TEXT("--------------------------------"));

    for (TFieldIterator<FProperty> It(Object->GetClass()); It; ++It)
    {
        FProperty* Property = *It;

        UE_LOG(LogTemp,Log,
            TEXT("%s (%s)"),
            *Property->GetName(),
            *Property->GetClass()->GetName());
    }

    UE_LOG(LogTemp, Log, TEXT("--------------------------------"));
}