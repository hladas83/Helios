
#include "../common.h"
#include "Entity.h"
#include "../Engine/Engine.h"

#include "SpaceObjects/Components/UIHelperComponents/UIBasicInfo.h"
#include "SpaceObjects/Components/UIHelperComponents/EntityIconComponent.h"
#include "SpaceObjects/Components/GenericComponents/OrbitersComponent.h"
#include "SpaceObjects/Components/UIHelperComponents/EntityOrbitComponent.h"


namespace Helios
{

  DEFINE_CASTING(Entity);

  //------------------------------------------------------------------------------  

  Entity::Entity()
  {
    InitClass();
  };

  //------------------------------------------------------------------------------  

  Entity::~Entity() 
  {
  };

  //------------------------------------------------------------------------------  

  Entity::Entity(Entity *parent, const HString &name) 
  {
    _parent = parent;
    _name = name;
    InitClass();
  };

  //------------------------------------------------------------------------------  

  void Entity::InitClass()
  {
    _entityConfig = GCoreConfig >> _name;

    // read simulation
    {
      HString simulationType = _entityConfig.ReadValue(HString("simulationType"), HString(""));
      WParamItem simulationCfg = _entityConfig >> "simulation";
      _movementSimulation = SimulationTypeFactory::ConstructData(simulationType);
      if (_movementSimulation)
        _movementSimulation->InitClass(this, simulationCfg);
      _size = _entityConfig.ReadValue("size", 1.0f);
    }
 
    // read all render objects
    WParamItem renderObjectsCfg = _entityConfig >> "renderObjects";
    if (renderObjectsCfg.IsClass())
    {
      ParamClass *renderObjectsClass = dyn_cast<ParamClass>(renderObjectsCfg.GetRawData());
      renderObjectsClass->ForEachItem([this](Ref<IParamItem> item)
      {
        WParamItem witem = WParamItem(item);
        WParamItem texturesCfg = witem >> "textures";

        std::vector<HString> textures;
        if (texturesCfg.IsArray())
        {
          for (int i = 0; i < texturesCfg.ArraySize(); ++i)
          {
            textures.push_back(texturesCfg.ReadArrayValue(i).GetValue<HString>());
          }
        }

        HString shape = witem.ReadValue(HString("shape"), HString("GameData/models/sphere.3DS"));
        HString shader = witem.ReadValue(HString("shader"), HString("GameData/FX/Planet.fx"));
        float scale = witem.ReadValue(HString("scale"), 1.0f);
        int renderPass = (int)(witem.ReadValue(HString("renderPass"), 0.0f));

        Matrix4 transform;
        transform.SetIdentityMatrix();
        transform.SetScale(scale);
        //todo enum cast
        _renderObjects.push_back(new RenderObject(shape, textures, shader, transform, (RenderObject::ERenderPass)renderPass));
        return false;
      });
    }

    // read all components
    WParamItem componentsCfg = _entityConfig >> "components";
    if (componentsCfg.IsClass())
    {
      ParamClass *componetsClass = dyn_cast<ParamClass>(componentsCfg.GetRawData());
      componetsClass->ForEachItem([this](Ref<IParamItem> item)
      {
        WParamItem witem = WParamItem(item);
        HString componentType = witem.ReadValue(HString("componentType"), HString(""));

        ComponentHolder *componentHolder = ComponentHolderFactory::ConstructData(componentType);
        if (componentHolder)
        {
          componentHolder->InitClass(this, witem);
          _componentList.push_back(componentHolder);
        }
        return false;
      });
    }
  }

  //------------------------------------------------------------------------------  

  void Entity::Simulate(float deltaT)
  {
    if (GetMovementSimulation())
    {
      GetMovementSimulation()->Simulate(this, deltaT);
    }
    ForEachComponent([deltaT](Ref<Component> component) {  component->Simulate(deltaT); });
  }

  //------------------------------------------------------------------------------  

  void Entity::PostSimulate()
  {
    if(_movementSimulation) _movementSimulation->PostSimulate();
    ForEachComponent([](Ref<Component> component) {  component->PostSimulate(); });
  }

  //------------------------------------------------------------------------------  

  void Entity::Draw()
  {
    for (int i = 0; i < GetRenderObjectCount(); ++i)
    {
      DrawContext context = DrawContext(GetRenderObject(i), GetRenderVisualState()->_frame);
      context.SetScale((1 + 0.04f*i)*_size);
      GEngine->GDraw()->RenderObject(DrawContext(context));
    }

    ForEachComponent([](Ref<Component> component) {  component->Draw(); });
  }

  //------------------------------------------------------------------------------  

  void Entity::PostDraw()
  {
    if(_movementSimulation) _movementSimulation->PostDraw();
    ForEachComponent([](Ref<Component> component) {  component->PostDraw(); });
  }

  //------------------------------------------------------------------------------  

  //! object movement simulation class
  PlainSimulation *Entity::GetMovementSimulation() const
  {
    return _movementSimulation;
  }

  //------------------------------------------------------------------------------  

  ObejctState *Entity::GetSimulationState()   
  {
    return _movementSimulation ? _movementSimulation->GetSimulationState() : nullptr;
  }

  //------------------------------------------------------------------------------  

  ObejctState *Entity::GetRenderVisualState() 
  {
    return _movementSimulation ? _movementSimulation->GetRenderVisualState() : nullptr;
  }

  //------------------------------------------------------------------------------  

  //! get object shape description
  Ref<RenderObject> Entity::GetRenderObject(unsigned int index) const 
  {
    return (index>= 0 && index<_renderObjects.size()) ? _renderObjects[index] : nullptr;
  };

  //------------------------------------------------------------------------------  

  int Entity::GetRenderObjectCount() const 
  {
    return _renderObjects.size();
  };

  //------------------------------------------------------------------------------  

  //!Set position (simulation state)
  void Entity::SetPosition(float x, float y, float z) 
  {
    if(_movementSimulation) _movementSimulation->SetPosition(x,y,z);
  };

  //------------------------------------------------------------------------------  

  void Entity::SetPosition(const Vector3 &position)   
  {
    if(_movementSimulation) _movementSimulation->SetPosition(position);
  };

} // Helios namespace