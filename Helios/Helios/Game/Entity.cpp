
#include "../common.h"
#include "Entity.h"
#include "../Engine/Engine.h"
#include "SpaceObjects/ObjectTypes/ObjectType.h"

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

  Entity::Entity(Entity *parent, ObjectType *type, const HString &name) : _parent(parent), _type(type)
  {
    _name = name;
    InitClass();
  };

  //------------------------------------------------------------------------------  

  void Entity::InitClass()
  {
    _entityConfig = GCoreConfig >> _name;

    HString simulationType = _entityConfig.ReadValue(HString("simulationType"), HString(""));
    WParamItem simulationCfg = _entityConfig >> "simulation"; 
    _movementSimulation = SimulationFactory::CreateSimulationClass(this, simulationType, simulationCfg);



    _size = _entityConfig.ReadValue("size", 1.0f);
  }

  //------------------------------------------------------------------------------  

  void Entity::ReInit()
  {
    InitClass();
  }

  //------------------------------------------------------------------------------  

  //!Get object type
  const ObjectType *Entity::GetType() const 
  {
    return _type;
  }

  //------------------------------------------------------------------------------  

  void Entity::Simulate(float deltaT)
  {
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