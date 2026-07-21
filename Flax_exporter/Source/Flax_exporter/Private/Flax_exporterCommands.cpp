// Copyright Epic Games, Inc. All Rights Reserved.

#include "Flax_exporterCommands.h"

#define LOCTEXT_NAMESPACE "FFlax_exporterModule"

void FFlax_exporterCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "Flax_exporter", "Execute Flax_exporter action", EUserInterfaceActionType::Button, FInputChord());
    // UI_COMMAND(GenerateMaterialColor,"GenerateMaterialColor","Generate representative material color",EUserInterfaceActionType::Button,FInputChord());
}

#undef LOCTEXT_NAMESPACE
