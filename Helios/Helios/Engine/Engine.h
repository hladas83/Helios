#pragma once

#ifndef _HELIOS_ENGINE
#define _HELIOS_ENGINE

//===================================================================================================
// included dependencies

#include "../common.h"
#include "IGame.h" 
#include "EngineCamera/ECamera.h"

#include "DX11Draw/DX11Draw.h" 
#include "FileServer/FileServer.h" 

namespace Helios
{

  //===================================================================================================
  // forward declared dependencies

  class ShapesBank;
  class ShadersBank;
  class TexturesBank;
  class UI;

  //===================================================================================================
  // Engine class


  class Engine : public Singleton<Engine>, public RefCount, public OLinkBase<Engine>
  {
  private:
    ECamera _eCamera;
    OLink<IGame> _iGame;

  public:
    Engine() {};
    ~Engine() {};
    bool Init();

#pragma region GMANAGERS
    //! rendering
    DX11Draw *GDraw() const;
    //! shapes bank
    ShapesBank *GShapes() const;
    //! fileserver
    FileServer *GFileServer() const;
    //! shaders
    ShadersBank *GShaders() const;
    //! shaders
    TexturesBank *GTextures() const;
    //! ui
    //UI *Engine::GUI() const;
    //! textures
    //__forceinline FileServer *GFileServer() const { return FileServer::GetFileServer(); }
#pragma endregion

    //delete all types
    void CleanUp();

    //-----------------------------------------------------------------------------
    // camera
    __forceinline void SetCamera(const Vector3 &eye, const Vector3 &at, const Vector3 &up) {return _eCamera.SetCamera(eye, at, up);};
    __forceinline void GetCameraMatrix(XMMATRIX  &g_View) {_eCamera.GetCameraMatrix(g_View);};
    __forceinline XMVECTOR GetCameraPosition() const {return _eCamera.GetCameraPosition();};

    //-----------------------------------------------------------------------------
    // game
    __forceinline void SetIGame(IGame *iGame) {_iGame = iGame;};
    __forceinline IGame *GetIGame() {return _iGame;};
    __forceinline const IGame *GetIGame() const {return _iGame;};

    USE_CASTING_ROOT;
  };

} // Helios namespace

#endif //_HELIOS_ENGINE