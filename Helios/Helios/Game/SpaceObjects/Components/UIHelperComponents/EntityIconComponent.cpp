#include "EntityIconComponent.h"
#include "../Engine/Engine.h"
#include "../Game/entity.h"

namespace Helios
{

  //------------------------------------------------------------------------------  

  EntityIconComponent::EntityIconComponent(Component *parent, WParamItem &entityConfig)
  {
    _parent = parent;

    HString infoType = entityConfig.ReadValue(HString("infoType"), HString("planetInfoType"));
    WParamItem infoTypeCfg = GCoreConfig >> infoType;

    std::vector<HString> textures;
    textures.push_back(infoTypeCfg.ReadValue(HString("icon"), HString("GameData/Textures/icons/planet.jpg")));

    Matrix4 transform;
    transform.SetIdentityMatrix();
    _iconRenderObject = new RenderObject("quad", textures, "GameData/FX/Icon.fx", transform, RenderObject::RPIcons, RenderObject::RTLine);
  }

  //------------------------------------------------------------------------------  

  EntityIconComponent::~EntityIconComponent()
  {

  }

  //------------------------------------------------------------------------------  

  void EntityIconComponent::Draw()
  {
    Entity *entity = dyn_cast<Entity>(_parent.GetObj());
    if (entity && _iconRenderObject)
    {
      DrawContext context = DrawContext(_iconRenderObject, entity->GetRenderVisualState()->_frame);
      context.SetScale(0.02f / GEngine->GDraw()->GetAspectRatio(), 0.02f, 1.0f);
      GEngine->GDraw()->RenderObject(DrawContext(context));
    }
  }

  //------------------------------------------------------------------------------  
}