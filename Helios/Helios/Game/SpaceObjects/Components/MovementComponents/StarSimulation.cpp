#include "StarSimulation.h"
#include "Entity.h"

namespace Helios
{

  DEFINE_CASTING(StarSimulation);

  //------------------------------------------------------------------------------

  StarSimulation::StarSimulation(Entity *simulationOwner, const  WParamItem &simulationCfg) : base(simulationOwner, simulationCfg)
  {
    InitClass();
  }

  //------------------------------------------------------------------------------

  StarSimulation::~StarSimulation() 
  {
  };

  //------------------------------------------------------------------------------

  void StarSimulation::InitClass()
  {
    //read config values
    _siderealRotationPeriod = _simulationConfig.ReadValue("siderealRotationPeriod", 0.0f);
  }

  //------------------------------------------------------------------------------  

  void StarSimulation::ReInit()
  {
    base::ReInit();
    InitClass();
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