// Copyright Epic Games, Inc. All Rights Reserved.

#include "SeaBattleGameMode.h"
#include "SeaBattleCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASeaBattleGameMode::ASeaBattleGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
