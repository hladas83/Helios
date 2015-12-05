//#pragma once
//
//#ifndef _HELIOS_UI
//#define _HELIOS_UI
//
////===================================================================================================
//// included dependencies
//
//#include "../../common.h"
//#include "CEGUI/String.h"
//// This header serves as a base for all samples and is needed inside the
//// SamplesFramework as interface for Samples that will be loaded.
//#include "CEGUI/ForwardRefs.h"
//
//struct ID3D11Device;
//struct ID3D11DeviceContext;
//
//
//namespace CEGUI
//{
//  class Window;
//  class DefaultWindow;
//
//  class GUIContext;
//  class TextureTarget;
//  class BasicImage;
//}
//
//namespace Helios
//{
//  //===================================================================================================
//  //HelloWorldDemo 
//
//  class HelloWorldDemo
//  {
//  public:
//    virtual bool initialise(CEGUI::GUIContext* guiContext);
//    virtual void deinitialise();
//
//  private:
//    bool handleHelloWorldClicked(const CEGUI::EventArgs& args);
//
//    CEGUI::DefaultWindow*      d_root;
//
//public:
//
//    virtual void update(float timeSinceLastUpdate) {}
//    const CEGUI::String& getUsedFilesString() {return d_usedFiles;}
//
//    CEGUI::String d_usedFiles;
//    HelloWorldDemo(){};
//  };
//
//  //===================================================================================================
//  // UI
//  class UI : public Singleton<UI>, public RefCount, public OLinkBase<UI> 
//  {
//  public:
//    CEGUI::GUIContext*      d_guiContext;
//    CEGUI::TextureTarget*   d_textureTarget;
//    CEGUI::BasicImage*      d_textureTargetImage; 
//
//  public:
//    UI();
//
//    void Init(){};
//    void SetDevice(ID3D11Device *device, ID3D11DeviceContext *context);
//  };
//
//} // Helios_UI namespace
//
//#endif // _HELIOS_UI