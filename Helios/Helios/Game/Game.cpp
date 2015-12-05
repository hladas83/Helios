//===================================================================================================
// included dependencies
#include "Game.h"
#include "Camera/Camera.h"
#include "SpaceDivisions/Space.h"
#include "../Engine/Engine.h"
#include "SpaceObjects/ObjectTypes/TypesBank.h"

namespace Helios
{

  DEFINE_CASTING(Game);

  //===================================================================================================

  Game::Game() 
  {
  };

  //------------------------------------------------------------------------------

  Game::~Game() 
  {
  };

  //------------------------------------------------------------------------------


  Space *Game::GSpace() const 
  {
    return Space::GetInstance();
  }

  //------------------------------------------------------------------------------

  TypesBank *Game::GTypes() const  
  {
    return TypesBank::GetInstance();
  }

  //------------------------------------------------------------------------------

  bool Game::Init() 
  {
    GSpace()->Init(); 
    GTypes()->Init(); 

    _camera = new Camera();
    return true;
  };

  //------------------------------------------------------------------------------

  //delete all types
  void Game::CleanUp()
  {
    Space::Release(); 
    TypesBank::Release(); 
  };

  //------------------------------------------------------------------------------

  //! main draw method
  void Game::Draw() 
  {
    if(GSpace()) 
    {
      GSpace()->Draw(); 

      if(_camera && GEngine)
      {
        _camera->UpdteCamera();
        GEngine->SetCamera(_camera->GetPosition(), _camera->GetLookAt(), _camera->GetDirUp());
      } 

      GSpace()->PostDraw();
    }
  };

  //------------------------------------------------------------------------------

  void Game::ProcessControls(UINT message, WPARAM wParam, LPARAM lParam)
  {
    switch (message)
    {
    case WM_MOUSEMOVE:
      if(GEngine)
      {
        int mouseX = ( short )LOWORD( lParam );
        int mouseY = ( short )HIWORD( lParam );
        if(_camera) _camera->MoveCamera(mouseX, mouseY);
      }
      break;

    case WM_MOUSEWHEEL:
      if(GEngine)
      {
        int mouseW =  (short)HIWORD( wParam );
        if(_camera) _camera->ZoomCamera(mouseW);
      }
      break;
    }

  }

  //------------------------------------------------------------------------------

  bool Game::IsGameRunning() const
  {
    return Space::IsRunning();
  }

} // Helios namespace
