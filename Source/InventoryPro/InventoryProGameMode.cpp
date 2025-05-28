// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventoryProGameMode.h"
#include "InventoryProCharacter.h"
#include "UObject/ConstructorHelpers.h"

AInventoryProGameMode::AInventoryProGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
