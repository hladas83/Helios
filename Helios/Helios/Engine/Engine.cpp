#include "Engine.h"
#include "Shapes/ShapesBank.h" 
#include "Shaders/ShadersBank.h" 
#include "Textures/TexturesBank.h" 
#include "UI/UI.h" 
#include "CEGUI/RendererModules/Direct3D11/Renderer.h"

namespace Helios
{

  //===================================================================================================
  DEFINE_CASTING(Engine);

  //! shapes bank
  ShapesBank *Engine::GShapes() const 
  {
    return ShapesBank::GetInstance();
  }

  //------------------------------------------------------------------------------

  ShadersBank *Engine::GShaders() const 
  {
    return ShadersBank::GetInstance(); 
  }

  //------------------------------------------------------------------------------

  TexturesBank *Engine::GTextures() const 
  {
    return TexturesBank::GetInstance(); 
  }

  //------------------------------------------------------------------------------

  DX11Draw *Engine::GDraw() const 
  {
    return DX11Draw::GetInstance();
  }


  //------------------------------------------------------------------------------

  FileServer *Engine::GFileServer() const 
  {
    return FileServer::GetInstance();
  } 

  //------------------------------------------------------------------------------

  UI *Engine::GUI() const 
  {
    return UI::GetInstance();
  } 

  //------------------------------------------------------------------------------

  bool Engine::Init() 
  {
    GFileServer()->LoadParams();

    GDraw()->Init(); 
    GShapes()->Init(); 
    GShaders()->Init();
    GTextures()->Init();
    GUI()->Init();
    return true;
  };

  //------------------------------------------------------------------------------

  //delete all types
  void Engine::CleanUp()
  {
    GShapes()->CleanupDX11Device();

    FileServer::Release();
    DX11Draw::Release();
    ShapesBank::Release();
    ShadersBank::Release();
    TexturesBank::Release();

    CEGUI::Direct3D11Renderer::destroySystem(); 
  };

} // Helios namespace