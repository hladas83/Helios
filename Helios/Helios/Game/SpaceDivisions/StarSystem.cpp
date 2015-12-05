#include "../../Common.h"
#include "StarSystem.h"
#include "../../Engine/Engine.h"
#include "../Game.h"

namespace Helios
{

  //===================================================================================================
  // StarSystem class

  DEFINE_CASTING(StarSystem)

  StarSystem::StarSystem() : Entity()
  {
  };

  //------------------------------------------------------------------------------

  StarSystem::StarSystem(Entity *parent, const HString &name) : Entity(parent, name)
  {
    InitClass();
  };

  //------------------------------------------------------------------------------  

  void StarSystem::InitClass()
  {
    //creare StarSystem render object   
    std::vector<HString> textures;
    textures.push_back(_entityConfig.ReadValue(HString("textureIn") ,HString("GameData/Textures/template.jpg")));

    Matrix4 transform;
    transform.SetIdentityMatrix();
    //star is just texture - so now used also for background
    _renderObjects.push_back(new RenderObject("GameData/models/sphereFliped.3DS", textures, "GameData/FX/Star.fx", transform ));
    _size = 10000.0f;

    //create all entities
    if(_entityConfig.IsClass())
    {
      //load stars
      const WParamItem itemStars = _entityConfig >> "stars";
      if(itemStars.IsArray())
      {
        for(int i = 0; i< itemStars.ArraySize(); ++i)
        {
          const WParamItem itemStarsArray = itemStars.ReadArrayValue(i);
          if(!itemStarsArray.IsString()) continue;

          //create star
          Ref<Entity> star = new Entity(this, itemStarsArray.GetValue(HString("")));
          if(star) _objects[ETStars].push_front(star);
        }
      }

      //load planets
      const WParamItem itemPlanets = _entityConfig >> "planets";
      if(itemPlanets.IsArray())
      {
        for(int i = 0; i< itemPlanets.ArraySize(); ++i)
        {
          const WParamItem itemPlanetsArray = itemPlanets.ReadArrayValue(i);
          if(!itemPlanetsArray.IsString()) continue;

          //create planet
          Ref<Entity> planet = new Entity(this, itemPlanetsArray.GetValue(HString("")));
          if(planet) _objects[ETPlanets].push_front(planet);
        }
      }
    }
  }

  //------------------------------------------------------------------------------

  StarSystem::~StarSystem()
  {
    __asm nop;    
  };

  //------------------------------------------------------------------------------

  // fidn entity with given config name
  Entity *StarSystem::FindEntity(const HString &name) const
  {
    //todo - make for each, that we can break, item is found
    Entity *entityMatch = nullptr;
    ForEachEntity([name, &entityMatch](Ref<Entity> entity) {  if( strcmpi(entity->GetName().data(), name.data()) == 0) { entityMatch = entity;};});
    return entityMatch;
  }

  //------------------------------------------------------------------------------

  void StarSystem::Simulate(float deltaT)
  {
    base::Simulate(deltaT);
    ForEachEntity([deltaT](Ref<Entity> entity) {  entity->Simulate(deltaT); });
  }

  //------------------------------------------------------------------------------

  void StarSystem::PostSimulate() 
  {
    base::PostSimulate();
    ForEachEntity([](Ref<Entity> entity) {  entity->PostSimulate(); });
  }

  //------------------------------------------------------------------------------

  void StarSystem::Draw()
  {
    CheckNull(GEngine);
    CheckNull(GEngine->GDraw());
    CheckNull(_movementSimulation);

    base::Draw();
    ForEachEntity([](Ref<Entity> entity) {  entity->Draw(); });
  }

  //------------------------------------------------------------------------------

  void StarSystem::PostDraw() 
  {
    base::PostDraw();
    ForEachEntity([](Ref<Entity> entity) {  entity->PostDraw(); });
  }

  //------------------------------------------------------------------------------

  void StarSystem::CleanUp()
  {
    for(int i = 0; i< ETCount; i++)
    {
      _objects[i].clear();
    }
  }

} // Helios namespace 