#include "StarSimulation.h"
#include "Entity.h"

namespace Helios
{

  DEFINE_CASTING(StarSimulation);
  DEF_SIMULATIONTYPE_FACTORY_REG(StarSimulation, HString("starsimulation"));

  //------------------------------------------------------------------------------

  StarSimulation::StarSimulation()
  {
  }

  //------------------------------------------------------------------------------

  StarSimulation::~StarSimulation() 
  {
  };

  //------------------------------------------------------------------------------

  void StarSimulation::InitClass(Entity *simulationOwner, const  WParamItem &simulationCfg)
  {
    base::InitClass(simulationOwner, simulationCfg);
    //read config values
    _siderealRotationPeriod = _simulationConfig.ReadValue("siderealRotationPeriod", 0.0f);
  }

  //------------------------------------------------------------------------------

  void StarSimulation::Simulate(Entity *simulationParent, float deltaT) 
  {
    base::Simulate(simulationParent, deltaT);

    CheckNull(simulationParent);
    ObejctState *simulationState = simulationParent->GetSimulationState();
    CheckNull(simulationState);

    Matrix4 rotation, translateToPosition;
    rotation.SetRotationY(_timeSimulated * _siderealRotationPeriod);

    translateToPosition.SetIdentityMatrix();
    translateToPosition.SetPosition(simulationState->_frame.GetPosition());

    simulationState->_frame = rotation * translateToPosition;
  }

  //------------------------------------------------------------------------------

} // Helios namespace