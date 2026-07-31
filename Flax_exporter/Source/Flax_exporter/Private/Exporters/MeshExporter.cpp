#include "Exporters/MeshExporter.h"

#include "Exporters/ActorExporter.h"
#include "Exporters/MaterialExporter.h"

#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"

#include "FlaxExportTypes.h"

void MeshExporter::Export(AStaticMeshActor* StaticMeshActor,FFlaxExportScene& Scene)
{
    if (!StaticMeshActor)
    {
        return;
    }

    UStaticMeshComponent* MeshComponent =
        StaticMeshActor->GetStaticMeshComponent();

    if (!MeshComponent)
    {
        return;
    }

    UStaticMesh* Mesh =
        MeshComponent->GetStaticMesh();

    if (!Mesh)
    {
        return;
    }

    //--------------------------------------
    // Static mesh
    //--------------------------------------

    FFlaxExportStaticMesh StaticMesh;

    StaticMesh.AssetPath = Mesh->GetPathName();
    StaticMesh.Name = Mesh->GetName();

    const int32 MeshIndex =
        Scene.StaticMeshes.AddUnique(StaticMesh);

    //--------------------------------------
    // Mesh instance
    //--------------------------------------

    FFlaxExportMeshInstance Instance;

    Instance.Name = StaticMeshActor->GetActorLabel();
    Instance.Folder = StaticMeshActor->GetFolderPath().ToString();

    if (Instance.Name.IsEmpty())
    {
        Instance.Name = StaticMeshActor->GetName();
    }

    Instance.Mesh = MeshIndex;

    const FTransform Transform =
        StaticMeshActor->GetActorTransform();

    Instance.Location = Transform.GetLocation();
    Instance.Rotation = Transform.GetRotation().Rotator();
    Instance.Scale = Transform.GetScale3D();

    Instance.Properties = ActorExporter::ExtractActorProperties(StaticMeshActor,MeshComponent);

    //--------------------------------------
    // Materials
    //--------------------------------------

    for (int32 i = 0; i < MeshComponent->GetNumMaterials(); ++i)
    {
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

        const int32 MaterialIndex = MaterialExporter::RegisterMaterial( Material, Scene);

        if (MaterialIndex >= 0)
        {
            Instance.Materials.Add(MaterialIndex);
        }
    }

    //--------------------------------------
    // Register instance
    //--------------------------------------

    Scene.MeshInstances.Add(Instance);
}