

//===================================================================================================
// included dependencies

#include "RenderObject.h"
#include "../../Engine/Engine.h"
#include "../Shapes/ShapesBank.h" 
#include "../Shaders/ShadersBank.h" 
#include "../Textures/TexturesBank.h" 


namespace Helios
{
  //===================================================================================================
  // RenderObject

  RenderObject::RenderObject(const HString &shapeName, std::vector<HString> textureNames, const HString &shaderName, Matrix4 transform,
    ERenderPass renderPass, ERenderType renderType) : 
    _renderPass(renderPass),
    _renderType(renderType),
    _transform(transform)
  {
    _shape = GEngine->GShapes() ? GEngine->GShapes()->GetLoadItem(shapeName) : nullptr;
    for(unsigned int i =0; i < textureNames.size(); i++)
    {
      Texture *texture = GEngine->GTextures() ? GEngine->GTextures()->GetLoadItem(textureNames[i]) : nullptr;
      if(texture) _textures.push_back(texture);
    }
    _shader = GEngine->GShaders()  ? GEngine->GShaders()->GetLoadItem(shaderName)  : nullptr;
  };

  //------------------------------------------------------------------------------ 

  RenderObject::RenderObject(const HString &shapeName,  const HString &shaderName, Matrix4 transform,
    ERenderPass renderPass, ERenderType renderType) : 
    _renderPass(renderPass),
    _renderType(renderType),
    _transform(transform)
  {
    _shape = GEngine->GShapes() ? GEngine->GShapes()->GetItem(shapeName) : nullptr;
    _shader = GEngine->GShaders()  ? GEngine->GShaders()->GetItem(shaderName)  : nullptr;
  };

  //------------------------------------------------------------------------------ 

    ID3D11ShaderResourceView *RenderObject::GetTexture(unsigned int index) const 
    {
      return  (index >=0 && index <  _textures.size()) ? _textures[index]->GetTexture() : nullptr;
    };

} // Helios namespace

