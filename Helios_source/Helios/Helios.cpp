#include <windows.h>

//===================================================================================================
// included dependencies

#include "../common.h"
#include "Elementary/TaskManager/TaskManager.h"
#include "Engine/Engine.h"
#include "Game/Game.h"
#include "Game/SpaceDivisions/Space.h" 
#include "Test/test.h"



 using namespace Helios;

//------------------------------------------------------------------------------

//!Initialize core classes
bool Init()
{
  // DX11 device, rendering, shapes, shaders,...
  if(!GEngine->Init()) 	return false;
  // game object and space
  if(!GGame->Init()) 	return false;
  // set interface for engine callbacks
  GEngine->SetIGame(GGame);

  return true;
}

//------------------------------------------------------------------------------  

//!Clean up core classes
void CleanUp()
{
  //release singletons
  Engine::Release();
  Game::Release();
}

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
  if(!Init()) exit(0);
  if(GGame && GGame->GSpace()) GGame->GSpace()->LoadGame();

  //start Rendering thread
  DX11DrawParams dX11DrawParams = DX11DrawParams(GEngine->GDraw(), 1, hInstance, hPrevInstance, lpCmdLine, nCmdShow);
  Ref<RenderTask> renderTask = new RenderTask(&dX11DrawParams);
  GTaskManager->SetRenderTask(renderTask);

  //start simulation thread
  SpaceSimulateTaskParams spaceSimulateTaskParams = SpaceSimulateTaskParams();
  Ref<SpaceSimulateTask> spaceSimulateTask = new SpaceSimulateTask(&spaceSimulateTaskParams);
  GTaskManager->SetSimualtionTask(spaceSimulateTask);

  //run render and simulation
  GTaskManager->RunMainThreads();

  //delete global variables
  CleanUp();

	exit(0);
}

