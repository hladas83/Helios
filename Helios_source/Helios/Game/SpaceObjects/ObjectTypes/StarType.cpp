#include "StarType.h"
#include "../Star.h"
#include "../SimulationMovementTypes/StarSimulation.h"

namespace Helios
{

  DEFINE_CASTING(StarType);

  //------------------------------------------------------------------------------  

  StarType::StarType(HString name) : base(name)
  {
    InitClass();
  };

  //------------------------------------------------------------------------------  

  StarType::~StarType() 
  {
  };

  //------------------------------------------------------------------------------  

  void StarType::InitClass()
  {

  }

  //------------------------------------------------------------------------------  

  void StarType::ReInit()
  {
    base::ReInit();
    InitClass();
  }

  //------------------------------------------------------------------------------  

  Entity *StarType::CreateObject(Entity *parent, const HString &name)
  {
    return new Star(this, parent, name);
  }

} // Helios namespace