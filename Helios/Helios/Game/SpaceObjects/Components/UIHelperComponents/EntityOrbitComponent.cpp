#include "EntityOrbitComponent.h"
#include "../Engine/Engine.h"
#include "../Game/entity.h"
#include "../Game/SpaceObjects/Components/MovementComponents/OrbiterSimulation.h"
#include "../Game/SpaceObjects/Components/UIHelperComponents/UIBasicInfo.h"

namespace Helios
{
  DEFINE_CASTING(EntityOrbitComponent);
  DEF_COMPONENTHOLDER_FACTORY_REG(EntityOrbitComponent, HString("entityorbitcomponent"));

  //------------------------------------------------------------------------------  

  EntityOrbitComponent::EntityOrbitComponent()
  {
  }

  //------------------------------------------------------------------------------  

  EntityOrbitComponent::~EntityOrbitComponent()
  {

  }

  //------------------------------------------------------------------------------  

  void EntityOrbitComponent::InitClass(ComponentHolder *parent, WParamItem &entityConfig)
  {
    _parent = parent;

    std::vector<HString> textures;
    Matrix4 transform;
    transform.SetIdentityMatrix();
    _renderOrbitObject = new RenderObject("circle", textures, "GameData/FX/Line.fx", transform, RenderObject::RPLine, RenderObject::RTLine);
  }

  //------------------------------------------------------------------------------  

  void EntityOrbitComponent::Draw()
  {
    Entity *entity = dyn_cast<Entity>(_parent.GetObj());
    if (entity && _renderOrbitObject)
    {
      const OrbiterSimulation *orbiretSim = dyn_cast<OrbiterSimulation> (entity->GetMovementSimulation());
      const Entity *gravityParent = orbiretSim ? orbiretSim->GetGravityParent() : nullptr;

      //render orbit trajectory
      if (orbiretSim && gravityParent)
      {
        UIBasicInfo *uiInfo = _parent->FindComponent<UIBasicInfo>();

        Matrix4 translateToPositionParent;
        translateToPositionParent.SetIdentityMatrix();
        translateToPositionParent.SetPosition(gravityParent->GetPosition<RenderState>());

        Matrix4 translateToFocus;
        translateToFocus.SetIdentityMatrix();
        translateToFocus.SetPosition(orbiretSim->GetExcentricity(), 0, 0);

        Matrix4 inclinationRotation;
        inclinationRotation.SetRotationZ(orbiretSim->GetInclination());
        translateToPositionParent = translateToFocus * inclinationRotation * translateToPositionParent;


        DrawContext context = DrawContext(_renderOrbitObject, translateToPositionParent);
        context.SetScale(orbiretSim->GetEllipseA(), 1, orbiretSim->GetEllipseB());
        context.SetAnimationPeriod(entity->GetRenderVisualState()->_animationPeriod);
        if(uiInfo)
          context.SetColor(uiInfo->GetInfoColor());

        GEngine->GDraw()->RenderObject(context);
      }

    }
  }

  //------------------------------------------------------------------------------  
}