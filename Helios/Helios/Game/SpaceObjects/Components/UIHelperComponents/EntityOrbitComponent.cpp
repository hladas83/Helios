#include "EntityOrbitComponent.h"
#include "../Engine/Engine.h"
#include "../Game/entity.h"
#include "../Game/SpaceObjects/Components/MovementComponents/OrbiterSimulation.h"

namespace Helios
{

  //------------------------------------------------------------------------------  

  EntityOrbitComponent::EntityOrbitComponent(ComponentHolder *parent, WParamItem &entityConfig)
  {
    _parent = parent;

    std::vector<HString> textures;
    Matrix4 transform;
    transform.SetIdentityMatrix();
    _renderOrbitObject = new RenderObject("circle", textures, "GameData/FX/Line.fx", transform, RenderObject::RPLine, RenderObject::RTLine);
  }

  //------------------------------------------------------------------------------  

  EntityOrbitComponent::~EntityOrbitComponent()
  {

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
      //  context.SetColor(_UIBasicInfo->GetInfoColor());

        GEngine->GDraw()->RenderObject(context);
      }

    }
  }

  //------------------------------------------------------------------------------  
}