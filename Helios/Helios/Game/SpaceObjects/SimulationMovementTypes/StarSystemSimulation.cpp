#include "StarSystemSimulation.h"
#include "../../Entity.h"
#include "../../../Game/Game.h"

namespace Helios
{

  DEFINE_CASTING(StarSystemSimulation);

  //------------------------------------------------------------------------------

  StarSystemSimulation::StarSystemSimulation(Entity *simulationOwner, const  WParamItem &simulationCfg) : base(simulationOwner, simulationCfg)
  {
    InitClass();
  }

  //------------------------------------------------------------------------------

  StarSystemSimulation::~StarSystemSimulation() 
  {
  };

  //------------------------------------------------------------------------------

  void StarSystemSimulation::InitClass()
  {
  }

  //------------------------------------------------------------------------------  

  void StarSystemSimulation::ReInit()
  {
    base::ReInit();
    InitClass();
  }

  //------------------------------------------------------------------------------

  void StarSystemSimulation::Simulate(Entity *simulationParent, float deltaT) 
  {
    base::Simulate(simulationParent, deltaT);

    CheckNull(simulationParent);
    ObejctState *simulationState = simulationParent->GetSimulationState();
    CheckNull(simulationState);

    Matrix4 translateToPosition;
    translateToPosition.SetIdentityMatrix();
    translateToPosition.SetPosition(GGame->GetCamera()->GetPosition());

    simulationState->_frame = translateToPosition;
  }

  //------------------------------------------------------------------------------

} // Helios namespace