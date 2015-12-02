#include "Star.h"
#include "../SpaceObjects/ObjectTypes/StarType.h"
#include "SpaceObjects/Components/MovementComponents/OrbiterSimulation.h"
#include "SpaceObjects/Components/UIHelperComponents/UIBasicInfo.h"
#include "../SpaceObjects/ObjectTypes/TypesBank.h"
#include "../../Engine/Shapes/Shape.h"
#include "../../Engine/Engine.h"
#include "../Game.h"

namespace Helios
{

  //------------------------------------------------------------------------------  

  DEFINE_CASTING(Star)

  //------------------------------------------------------------------------------  

  Star::Star(ObjectType *type, Entity *parent, const HString &name): base(parent, type, name) 
  {
    InitClass();
  };

  //------------------------------------------------------------------------------  

  void Star::InitClass()
  {
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

        HString shape = witem.ReadValue(HString("shape"), HString("GameData/models/sphere.3DS"));
        HString shader = witem.ReadValue(HString("shader"), HString("GameData/FX/Star.fx"));
        float scale =     witem.ReadValue(HString("scale"), 1.0f);
        int renderPass = (int)(witem.ReadValue(HString("renderPass"),  0.0f));

        Matrix4 transform;
        transform.SetIdentityMatrix();
        _renderObjects.push_back(new RenderObject(shape, textures, shader, transform));   
        return false;
      });
    }


    //create all orbiters
    if(_entityConfig.IsClass())
    {
      //load stars
      const WParamItem itemOrbiters = _entityConfig >> "orbiters";
      if(itemOrbiters.IsArray())
      {
        for(int i = 0; i< itemOrbiters.ArraySize(); ++i)
        {
          const WParamItem itemOrbitersArray = itemOrbiters.ReadArrayValue(i);
          if(!itemOrbitersArray.IsString()) continue;

          //create star
          Ref<Entity> orbiter = GGame->GTypes()->CreateObject(this, itemOrbitersArray.GetValue(HString("")));
          if(orbiter) 
          {
            _orbiters.push_front(orbiter);
            if(orbiter->GetMovementSimulation())
            {
              OrbiterSimulation *orbiretSim = dyn_cast<OrbiterSimulation> (orbiter->GetMovementSimulation());
              orbiretSim->SetGravityParent(this);
            }

            if( strcmpi(orbiter->GetName().data(), "earth") == 0)
            {
              GGame->GetCamera()->SetTarget(orbiter);
            }
          }
        }
      }
    }
  }

  //------------------------------------------------------------------------------  

  void Star::ReInit()
  {
    base::ReInit();
    InitClass();
  }

  //------------------------------------------------------------------------------  

  Star::~Star() 
  {
  };

  //------------------------------------------------------------------------------  

  void Star::Simulate(float deltaT)
  {
    CheckNull(GetType());
    base::Simulate(deltaT);

    ForEachOrbiter([deltaT](Ref<Entity> entity) {  entity->Simulate(deltaT); });
  }

  //------------------------------------------------------------------------------

  void Star::PostSimulate()
  {
    base::PostSimulate();
    ForEachOrbiter([](Ref<Entity> entity) {  entity->PostSimulate(); });
  }

  //------------------------------------------------------------------------------  

  void Star::Draw()
  {
    CheckNull(GEngine);
    CheckNull(GEngine->GDraw());
    base::Draw();

    DrawContext context = DrawContext(GetRenderObject(0), GetRenderVisualState()->_frame);
    context.SetScale(_size);

    GEngine->GDraw()->RenderObject(context);
    ForEachOrbiter([](Ref<Entity> entity) {  entity->Draw(); });
  }

  //------------------------------------------------------------------------------

  void Star::PostDraw()
  {
    base::PostDraw();
    ForEachOrbiter([](Ref<Entity> entity) {  entity->PostDraw(); });
  }

  //------------------------------------------------------------------------------  

  const StarType *Star::Type() const
  {
    return dyn_cast<StarType>(_type.GetRef());
  }

} // Helios namespace