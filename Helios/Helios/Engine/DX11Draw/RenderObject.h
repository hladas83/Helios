#pragma once

#ifndef _HELIOS_RENDER_OBJECT
#define _HELIOS_RENDER_OBJECT

//===================================================================================================
// included dependencies

#include "../../common.h"

namespace Helios
{

  //===================================================================================================
  // forward declared dependencies

  class Shape;
  class Texture;
  class Shader;


  //===================================================================================================
  // RenderObject

  class RenderObject : public RefCount, public OLinkBase<RenderObject>
  {
  private:
    Ref<Shape>  _shape;
    Ref<Shader> _shader;
    std::vector<Ref<Texture>> _textures;
    Matrix4 _transform;   

  //------------------------------------------------------------------------------ 
  public:
    enum ERenderPass
    {
      RPStandart,
      RPTransparent,
      RPLine,
      RPIcons,
      RPCount
    } _renderPass;

    //------------------------------------------------------------------------------ 
    enum ERenderType
    {
      RTStandart,
      RTStrip,
      RTLine,
      RTCount
    } _renderType;
    //------------------------------------------------------------------------------ 

  public:
    RenderObject(const HString &shapeName, std::vector<HString> textureNames, const HString &shaderName, Matrix4 transform, 
      ERenderPass renderPass = RPStandart, ERenderType renderType = RTStandart);

    RenderObject(const HString &shapeName, const HString &shaderName, Matrix4 transform, 
      ERenderPass renderPass = RPStandart, ERenderType renderType = RTStandart);

    //------------------------------------------------------------------------------ 
    //! get renderObject info
    Shape* GetShape() const {return _shape;};
    ID3D11ShaderResourceView *GetTexture(unsigned int index) const ;
    int GetTexturesCount(int index) const {return _textures.size();};
    Shader *GetShader() const {return _shader;};
    Matrix4 GetTransform() const {return _transform;};
  };

} // Helios namespace

#endif // _HELIOS_DRAW_TASK