#include "Orbiter.h"
#include "../SpaceObjects/ObjectTypes/OrbiterType.h"
#include "../SpaceObjects/ObjectTypes/TypesBank.h"
#include "SpaceObjects/Components/MovementComponents/OrbiterSimulation.h"
#include "SpaceObjects/Components/UIHelperComponents/UIBasicInfo.h"
#include "SpaceObjects/Components/UIHelperComponents/EntityOrbitComponent.h"
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


  }

  //------------------------------------------------------------------------------

  void Orbiter::PostSimulate()
  {
    base::PostSimulate();
  }

  //------------------------------------------------------------------------------

  void Orbiter::Draw()
  {
    CheckNull(GEngine);
    CheckNull(GEngine->GDraw());
    base::Draw();
  }

  //------------------------------------------------------------------------------

  void Orbiter::PostDraw()
  {
    base::PostDraw();
  }

  //------------------------------------------------------------------------------

  //return real type
  const OrbiterType *Orbiter::Type() const 
  {
    return dyn_cast<OrbiterType>(_type.GetRef());
  }

} //namespace Helios