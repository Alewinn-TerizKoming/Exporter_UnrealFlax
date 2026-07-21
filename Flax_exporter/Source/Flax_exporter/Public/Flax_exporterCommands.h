// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Framework/Commands/Commands.h"
#include "Flax_exporterStyle.h"

class FFlax_exporterCommands : public TCommands<FFlax_exporterCommands>
{
public:

	FFlax_exporterCommands()
		: TCommands<FFlax_exporterCommands>(TEXT("Flax_exporter"), NSLOCTEXT("Contexts", "Flax_exporter", "Flax_exporter Plugin"), NAME_None, FFlax_exporterStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
	TSharedPtr<FUICommandInfo> GenerateMaterialColor;
};
