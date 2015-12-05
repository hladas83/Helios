#include "Star.h"
#include "../SpaceObjects/ObjectTypes/StarType.h"

#include "SpaceObjects/Components/MovementComponents/OrbiterSimulation.h"
#include "SpaceObjects/Components/UIHelperComponents/UIBasicInfo.h"
#include "SpaceObjects/Components/GenericComponents/OrbitersComponent.h"

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
  }

  //------------------------------------------------------------------------------

  void Star::PostSimulate()
  {
    base::PostSimulate();
  }

  //------------------------------------------------------------------------------  

  void Star::Draw()
  {
    CheckNull(GEngine);
    CheckNull(GEngine->GDraw());
    base::Draw();
  }

  //------------------------------------------------------------------------------

  void Star::PostDraw()
  {
    base::PostDraw();
  }

  //------------------------------------------------------------------------------  

  const StarType *Star::Type() const
  {
    return dyn_cast<StarType>(_type.GetRef());
  }

} // Helios namespace