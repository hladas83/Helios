

//===================================================================================================
// included dependencies

#include "RenderManager.h"
#include "RenderObject.h"
#include  "../Engine.h"

namespace Helios
{

  //===================================================================================================
  // DrawContext class

  DrawContext::DrawContext(RenderObject *renderObject, Matrix4 transform) :
    _renderObject(renderObject),
    _transform(transform),
    _sizeX(1),
    _sizeY(1),
    _sizeZ(1),
    _animationPeriod(0),
    _defaultColor(1,1,1,1)
  {};


  //------------------------------------------------------------------------------

  DrawContext::~DrawContext()
  {

  }

  //------------------------------------------------------------------------------

  //sorting function
  float DrawContext::GetDephDistance() const
  {
    return  GEngine->GDraw()->WorldToScreen(_transform.GetPosition()).Z();
  }

  //===================================================================================================
  // RenderManager class

  RenderManager::RenderManager()
  {
  };

  //------------------------------------------------------------------------------

  RenderManager::~RenderManager()
  {
  };

  //------------------------------------------------------------------------------

  void RenderManager::RenderObject(DrawContext &renderContex)
  {
    _renderLists[renderContex._renderObject->_renderPass].push_back(renderContex);
  }

  //------------------------------------------------------------------------------

  void RenderManager::RenderAll()
  {
    //todo
    std::for_each(_renderLists[RenderObject::RPStandart].begin(), _renderLists[RenderObject::RPStandart].end(), [](DrawContext drawContext) {  GEngine->GDraw()->RenderShape(drawContext); });
    std::for_each(_renderLists[RenderObject::RPTransparent].begin(), _renderLists[RenderObject::RPTransparent].end(), [](DrawContext drawContext) {  GEngine->GDraw()->RenderShape(drawContext); });
    std::for_each(_renderLists[RenderObject::RPLine].begin(), _renderLists[RenderObject::RPLine].end(), [](DrawContext drawContext) {GEngine->GDraw()->RenderLine(drawContext); });
    
    GEngine->GDraw()->ClearZBuffer();
    _renderLists[RenderObject::RPIcons].sort([](const DrawContext &a, const DrawContext &b) -> bool{ return a.GetDephDistance() > b.GetDephDistance(); });
    std::for_each(_renderLists[RenderObject::RPIcons].begin(), _renderLists[RenderObject::RPIcons].end(), [](DrawContext drawContext) {  GEngine->GDraw()->RenderIcon(drawContext); });

    for(int i = 0 ; i < RenderObject::RPCount; ++i) _renderLists[i].clear();
  }

} // Helios namespace

