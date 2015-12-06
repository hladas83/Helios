#include "OrbitersComponent.h"
#include "../Engine/Engine.h"
#include "../Game/Game.h"
#include "../Game/entity.h"
#include "../Game/SpaceObjects/Components/MovementComponents/OrbiterSimulation.h"

namespace Helios
{
  DEFINE_CASTING(OrbitersComponent);
  DEF_COMPONENTHOLDER_FACTORY_REG(OrbitersComponent, HString("orbiterscomponent"));

  //------------------------------------------------------------------------------  

  OrbitersComponent::OrbitersComponent()
  {
  }

  //------------------------------------------------------------------------------  

  OrbitersComponent::~OrbitersComponent()
  {

  }

  //------------------------------------------------------------------------------  

  void OrbitersComponent::InitClass(ComponentHolder *parent, WParamItem &entityConfig)
  {
    _parent = parent;
    Entity *entity = dyn_cast<Entity>(_parent.GetObj());

    //create all orbiters
    if (entity && entityConfig.IsClass())
    {
      //load stars
      const WParamItem itemOrbiters = entityConfig >> "orbiters";
      if (itemOrbiters.IsArray())
      {
        for (int i = 0; i< itemOrbiters.ArraySize(); ++i)
        {
          const WParamItem itemOrbitersArray = itemOrbiters.ReadArrayValue(i);
          if (!itemOrbitersArray.IsString()) continue;

          //create star
          Ref<Entity> orbiter = new Entity(entity, itemOrbitersArray.GetValue(HString("")));
          if (orbiter)
          {
            _orbiters.push_front(orbiter);
            if (orbiter->GetMovementSimulation())
            {
              OrbiterSimulation *orbiretSim = dyn_cast<OrbiterSimulation> (orbiter->GetMovementSimulation());
              orbiretSim->SetGravityParent(entity);
            }

            if (strcmpi(orbiter->GetName().data(), "sun") == 0)
            {
              GGame->GetCamera()->SetTarget(orbiter);
            }
          }
        }
      }
    }
  }

  //------------------------------------------------------------------------------  

  void OrbitersComponent::Simulate(float deltaT)
  {
    base::Simulate(deltaT);
    ForEachOrbiter([deltaT](Ref<Entity> entity) {  entity->Simulate(deltaT); });
  }

  //------------------------------------------------------------------------------

  void OrbitersComponent::PostSimulate()
  {
    base::PostSimulate();
    ForEachOrbiter([](Ref<Entity> entity) {  entity->PostSimulate(); });
  }

  //------------------------------------------------------------------------------  

  void OrbitersComponent::Draw()
  {
    CheckNull(GEngine);
    CheckNull(GEngine->GDraw());

    base::Draw();
    ForEachOrbiter([](Ref<Entity> entity) {  entity->Draw(); });
  }

  //------------------------------------------------------------------------------

  void OrbitersComponent::PostDraw()
  {
    base::PostDraw();
    ForEachOrbiter([](Ref<Entity> entity) {  entity->PostDraw(); });
  }

  //------------------------------------------------------------------------------  
}