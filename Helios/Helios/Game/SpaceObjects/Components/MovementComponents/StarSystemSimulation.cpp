#include "StarSystemSimulation.h"
#include "Entity.h"
#include "Game.h"

namespace Helios
{

  DEFINE_CASTING(StarSystemSimulation);
  DEF_SIMULATIONTYPE_FACTORY_REG(StarSystemSimulation, HString("starsystemsimulation"));

  //------------------------------------------------------------------------------

  StarSystemSimulation::StarSystemSimulation()
  {

  } 

  //------------------------------------------------------------------------------

  StarSystemSimulation::~StarSystemSimulation() 
  {
  };

  //------------------------------------------------------------------------------

  void StarSystemSimulation::InitClass(Entity *simulationOwner, const  WParamItem &simulationCfg)
  {    
    base::InitClass(simulationOwner, simulationCfg);
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