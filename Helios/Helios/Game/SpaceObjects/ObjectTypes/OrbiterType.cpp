#include "OrbiterType.h"
#include "../Orbiter.h"
#include "SpaceObjects/Components/MovementComponents/OrbiterSimulation.h"

#include "../../Engine/Engine.h"

namespace Helios
{

  DEFINE_CASTING(OrbiterType);

  //------------------------------------------------------------------------------  

  OrbiterType::OrbiterType(HString name) : base(name)
  {
    InitClass();
  };

  //------------------------------------------------------------------------------  

  OrbiterType::~OrbiterType() 
  {
  };

  //------------------------------------------------------------------------------  

  void OrbiterType::InitClass()
  {
    std::vector<HString> textures;
    Matrix4 transform;
    transform.SetIdentityMatrix();
    _renderOrbitObject = new RenderObject("circle",textures,"GameData/FX/Line.fx", transform, RenderObject::RPLine, RenderObject::RTLine);
  }

  //------------------------------------------------------------------------------  

  void OrbiterType::ReInit()
  {
    base::ReInit();
    InitClass();
  }

  //------------------------------------------------------------------------------  

  Entity *OrbiterType::CreateObject(Entity *parent, const HString &name)
  {
    return new Orbiter(this, parent, name);
  }

} // Helios namespace