// Copyright Epic Games, Inc. All Rights Reserved.

#include "Flax_exporterStyle.h"
#include "Flax_exporter.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FFlax_exporterStyle::StyleInstance = nullptr;

void FFlax_exporterStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FFlax_exporterStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FFlax_exporterStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("Flax_exporterStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon24x24(24.0f, 24.0f);

TSharedRef< FSlateStyleSet > FFlax_exporterStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("Flax_exporterStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("Flax_exporter")->GetBaseDir() / TEXT("Resources"));

	Style->Set("Flax_exporter.PluginAction", new IMAGE_BRUSH_SVG(TEXT("ExpFl_20_Svg"), Icon24x24));
	return Style;
}

void FFlax_exporterStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FFlax_exporterStyle::Get()
{
	return *StyleInstance;
}
