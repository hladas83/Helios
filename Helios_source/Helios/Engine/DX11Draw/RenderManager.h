#pragma once

#ifndef _HELIOS_RENDER_MANAGER
#define _HELIOS_RENDER_MANAGER

//===================================================================================================
// included dependencies

#include "../../common.h"
#include "RenderObject.h"

namespace Helios
{

  class RenderObject;

  //===================================================================================================
  //! Render Context 
  class DrawContext
  {
  public:
    Ref<RenderObject>       _renderObject;                    //! geometry to render
    Matrix4                 _transform;                       //! object transformation
    float                   _sizeX,_sizeY, _sizeZ;            //! object scale
    float                   _animationPeriod;                 //! current animation phase
    HColor                  _defaultColor;                    //! object base colors

    DrawContext(RenderObject *renderObject, Matrix4 transform);

    //-----------------------------------------------------------------------------
    // properties
    void SetColor(HColor color) {_defaultColor = color;};
    void SetAnimationPeriod(float animationPeriod) {_animationPeriod = animationPeriod;};
    void SetScale(float scale) {_sizeX = _sizeY = _sizeZ = scale; };
    void SetScale(float scaleX, float scaleY, float scaleZ) {_sizeX = scaleX; _sizeY = scaleY; _sizeZ = scaleZ; };

    //sorting function
    float GetDephDistance() const;

    ~DrawContext();
  };

  //===================================================================================================
  // Render Manager 
  
  class RenderManager
  {

  private:
    std::list<DrawContext> _renderLists[RenderObject::RPCount];

  public:
    RenderManager();
    ~RenderManager();

    void RenderObject(DrawContext &renderContex);
    void RenderAll();
  };

} // Helios namespace

#endif // _HELIOS_RENDER_MANAGER