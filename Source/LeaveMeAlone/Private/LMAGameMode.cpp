// LeaveMeAlone Game by Netologiya. All RightsReserved.


#include "LMAGameMode.h"
#include "PLayer/LMADefaultCharacter.h"
#include "PLayer/LMAPlayerController.h"

ALMAGameMode::ALMAGameMode()
{
	DefaultPawnClass = ALMADefaultCharacter::StaticClass();
	PlayerControllerClass = ALMAPlayerController::StaticClass();
}