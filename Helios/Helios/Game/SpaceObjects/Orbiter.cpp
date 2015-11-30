#include "Orbiter.h"
#include "../SpaceObjects/ObjectTypes/OrbiterType.h"
#include "../SpaceObjects/ObjectTypes/TypesBank.h"
#include "SpaceObjects/Components/MovementComponents/OrbiterSimulation.h"
#include "SpaceObjects/Components/UIHelperComponents/UIBasicInfo.h"
#include "../../Engine/Engine.h"
#include "../../Engine/DX11Draw/RenderObject.h"
#include "../Game.h"

namespace Helios
{

  //===================================================================================================
  // simulate thread methods

  DEFINE_CASTING(Orbiter)

  Orbiter::Orbiter(ObjectType *type, Entity *parent, const HString &name): base(parent, type, name) 
  {
    InitClass();
  };

  //------------------------------------------------------------------------------  

  void Orbiter::InitClass()
  {
    //TODO - move to entity
    // read all render objects
    WParamItem renderObjectsCfg = _entityConfig >> "renderObjects";
    if(renderObjectsCfg.IsClass())
    {
      ParamClass *renderObjectsClass = dyn_cast<ParamClass>(renderObjectsCfg.GetRawData());
      renderObjectsClass->ForEachItem([this](Ref<IParamItem> item) 
      {  
        WParamItem witem = WParamItem(item);
        WParamItem texturesCfg = witem >> "textures";
        
        std::vector<HString> textures;
        if(texturesCfg.IsArray())
        {
          for (int i = 0; i < texturesCfg.ArraySize(); ++i)
          {
            textures.push_back(texturesCfg.ReadArrayValue(i).GetValue<HString>());
          }
        }

        HString shape =   witem.ReadValue(HString("shape"), HString("GameData/models/sphere.3DS"));
        HString shader =  witem.ReadValue(HString("shader"), HString("GameData/FX/Planet.fx"));
        float scale =     witem.ReadValue(HString("scale"), 1.0f);
        int renderPass = (int)(witem.ReadValue(HString("renderPass"),  0.0f));

        Matrix4 transform;
        transform.SetIdentityMatrix();
        transform.SetScale(scale);
        //todo enum cast
        _renderObjects.push_back(new RenderObject(shape, textures, shader, transform, (RenderObject::ERenderPass)renderPass));   
        return false;
      });
    }


    //create all orbiters
    if(_entityConfig.IsClass())
    {
      //load orbiters
      const WParamItem itemOrbiters = _entityConfig >> "orbiters";
      if(itemOrbiters.IsArray())
      {
        for(int i = 0; i< itemOrbiters.ArraySize(); ++i)
        {
          const WParamItem itemOrbitersArray = itemOrbiters.ReadArrayValue(i);
          if(!itemOrbitersArray.IsString()) continue;

          //create orbiters
          Ref<Entity> orbiter = GGame->GTypes()->CreateObject(this, itemOrbitersArray.GetValue(HString("")));
          if(orbiter) 
          {
            _orbiters.push_front(orbiter);
            if(orbiter->GetMovementSimulation())
            {
              OrbiterSimulation *orbiretSim = dyn_cast<OrbiterSimulation> (orbiter->GetMovementSimulation().GetRef());
              orbiretSim->SetGravityParent(this);
            }
          }
        }
      }
    }
  }

  //------------------------------------------------------------------------------  

  void Orbiter::ReInit()
  {
    base::ReInit();
    InitClass();
  }

  //------------------------------------------------------------------------------

  Orbiter::~Orbiter() 
  {
  };

  //------------------------------------------------------------------------------

  void Orbiter::Simulate(float deltaT)
  {
    CheckNull(GetType());
    base::Simulate(deltaT);

     ForEachOrbiter([deltaT](Ref<Entity> entity) {  entity->Simulate(deltaT); });
  }

  //------------------------------------------------------------------------------

  void Orbiter::PostSimulate()
  {
    base::PostSimulate();
    ForEachOrbiter([](Ref<Entity> entity) {  entity->PostSimulate(); });
  }

  //------------------------------------------------------------------------------

  void Orbiter::Draw()
  {
    CheckNull(GEngine);
    CheckNull(GEngine->GDraw());
    base::Draw();

    for(int i = 0; i < GetRenderObjectCount(); ++i) 
    {
      DrawContext context = DrawContext(GetRenderObject(i), GetRenderVisualState()->_frame);
      context.SetScale((1 + 0.04f*i)*_size);
      GEngine->GDraw()->RenderObject(DrawContext(context));
    }

    const OrbiterSimulation *orbiretSim = dyn_cast<OrbiterSimulation> (_movementSimulation.GetRef());
    const Entity *gravityParent =  orbiretSim ? orbiretSim->GetGravityParent() : nullptr;

    //render orbit trajectory
    if(orbiretSim && gravityParent)
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

      Ref<RenderObject> renderOrbitObject = Type()->GetRenderOrbitObject();

      DrawContext context = DrawContext(renderOrbitObject, translateToPositionParent);
      context.SetScale(orbiretSim->GetEllipseA(), 1, orbiretSim->GetEllipseB());
      context.SetAnimationPeriod(GetRenderVisualState()->_animationPeriod );
      context.SetColor(_UIBasicInfo->GetInfoColor());

      if(renderOrbitObject) GEngine->GDraw()->RenderObject(context);
    }

    ForEachOrbiter([](Ref<Entity> entity) {  entity->Draw(); });
  }

  //------------------------------------------------------------------------------

  void Orbiter::PostDraw()
  {
    base::PostDraw();
    ForEachOrbiter([](Ref<Entity> entity) {  entity->PostDraw(); });
  }

  //------------------------------------------------------------------------------

  //return real type
  const OrbiterType *Orbiter::Type() const 
  {
    return dyn_cast<OrbiterType>(_type.GetRef());
  }

} //namespace Helios