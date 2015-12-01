#include "OrbiterSimulation.h"
#include "Entity.h"
#include "SpaceObjects/Orbiter.h"
#include "../../../Engine/Engine.h"

namespace Helios
{

  DEFINE_CASTING(OrbiterSimulation);

  //------------------------------------------------------------------------------

  OrbiterSimulation::OrbiterSimulation(Entity *simulationOwner, const  WParamItem &simulationCfg) : base(simulationOwner, simulationCfg)
  {
    InitClass();
  };

  //------------------------------------------------------------------------------

  OrbiterSimulation::~OrbiterSimulation() 
  {
  };

  //------------------------------------------------------------------------------

  void OrbiterSimulation::InitClass()
  {
    //read config values
    _siderealRotationPeriod = _simulationConfig.ReadValue("siderealRotationPeriod", 0.0f); 
    _orbitalPeriod = _simulationConfig.ReadValue("orbitalPeriod", 0.0f);

    _aphelion = _simulationConfig.ReadValue("aphelion", 0.0f);
    _perihelion = _simulationConfig.ReadValue("perihelion", 0.0f);
    Assert(_aphelion >= _perihelion);

    _ellipseA = (_aphelion + _perihelion) / 2;
    _excentricity = _ellipseA - _perihelion;
    _ellipseB = sqrtf(pow(_ellipseA, 2) - pow(_excentricity, 2));
    
		_inclination = DEG2RAD(_simulationConfig.ReadValue("inclination", 0.0f));
  }

  //------------------------------------------------------------------------------  

  void OrbiterSimulation::ReInit()
  {
    base::ReInit();
    InitClass();
  }

  //------------------------------------------------------------------------------

  const Entity *OrbiterSimulation::GetGravityParent() const
  {
    return _gravityParent;
  };

  //------------------------------------------------------------------------------

  void OrbiterSimulation::SetGravityParent(Entity *gravityParent)
  {
    _gravityParent = gravityParent;
  };

  //------------------------------------------------------------------------------

  void OrbiterSimulation::Simulate(Entity *simulationParent, float deltaT) 
  {
    base::Simulate(simulationParent, deltaT);

    CheckNull(simulationParent);
    ObejctState *simulationState = simulationParent->GetSimulationState();
    CheckNull(simulationState);
    CheckNull(_gravityParent);

    Matrix4 rotation, inclinationRotation, translateToPosition, translateToPositionParent;
    rotation.SetRotationY(_timeSimulated * _siderealRotationPeriod);

    translateToPosition.SetIdentityMatrix();
    translateToPosition.SetPosition( _excentricity + cos(_timeSimulated * _orbitalPeriod ) * _ellipseA, 0, -sin(_timeSimulated * _orbitalPeriod ) * _ellipseB);

    inclinationRotation.SetRotationZ(_inclination);
    translateToPosition = translateToPosition * inclinationRotation;

    translateToPositionParent.SetIdentityMatrix();
    translateToPositionParent.SetPosition(_gravityParent->GetPosition<SimulationState>());

    simulationState->_frame = rotation * translateToPosition * translateToPositionParent;
    simulationState->_animationPeriod = fmod(_timeSimulated * _orbitalPeriod / PI_2, 1.0f);
  }

  //------------------------------------------------------------------------------

} // Helios namespace