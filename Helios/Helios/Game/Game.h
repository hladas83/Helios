#pragma once

#ifndef _HELIOS_GAME
#define _HELIOS_GAME

//===================================================================================================
// included dependencies

#include "../common.h"
#include "../Engine/IGame.h"
#include "Camera/Camera.h"

namespace Helios
{

  //===================================================================================================
  // forward declared dependencies

  class Space;

  //===================================================================================================
  // Game class

  class Game : public  Singleton<Game>, public IGame
  {
  private:
    Ref<ICamera> _camera;

  public:
    Game();
    virtual ~Game();
    bool Init();

    //! world representation
    __forceinline Space *GSpace() const;

    //!Delete all types and objects
    void CleanUp();

    //! main draw method
    virtual void Draw() override;
    virtual void ProcessControls(UINT message, WPARAM wParam, LPARAM lParam) override;
    virtual bool IsGameRunning() const override;

    __forceinline ICamera *GetCamera() const {return _camera;};

    USE_CASTING_ROOT;
  };

} // Helios namespace

#endif // _HELIOS_GAME