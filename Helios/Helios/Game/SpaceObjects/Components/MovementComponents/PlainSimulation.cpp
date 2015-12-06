#include "PlainSimulation.h"
#include "StarSimulation.h"
#include "OrbiterSimulation.h"
#include "StarSystemSimulation.h"
#include "../../Engine/Engine.h"
#include "Entity.h"

namespace Helios
{

  //------------------------------------------------------------------------------  

  PlainSimulation::PlainSimulation() : 
    _timeSimulated(0),
    _currentRenderState(0)
  {
    _renderVisualState[0] = new ObejctState();
    _renderVisualState[1] = new ObejctState();
    _simulationState = new ObejctState();
  };

  //------------------------------------------------------------------------------  

  PlainSimulation::~PlainSimulation() 
  {

  };

  //------------------------------------------------------------------------------  

  void PlainSimulation::InitClass(Entity *simulationOwner, const  WParamItem &simulationCfg)
  {
    _simulationOwner = simulationOwner;
    _simulationConfig = simulationCfg;
  }


  //------------------------------------------------------------------------------  

  void PlainSimulation::PostSimulate()
  {
    int stateToUpdate = (_currentRenderState + 1) % 2;

    _renderVisualState[stateToUpdate]->_frame = _simulationState->_frame;
    _renderVisualState[stateToUpdate]->_animationPeriod = _simulationState->_animationPeriod;
    _renderVisualState[stateToUpdate]->_valid = true;
  }

  //------------------------------------------------------------------------------  

  void PlainSimulation::PostDraw()
  {
    int next = (_currentRenderState + 1) % 2;
    _renderVisualState[_currentRenderState]->_valid = false;
    if(_renderVisualState[next]->_valid) _currentRenderState = next;
  }

  //------------------------------------------------------------------------------  

  void PlainSimulation::SetPosition(float x, float y, float z)
  {
    _simulationState->_frame.SetPosition(x,y,z);
    _renderVisualState[0]->_frame.SetPosition(x,y,z);
    _renderVisualState[1]->_frame.SetPosition(x,y,z);
  };

  void PlainSimulation::SetPosition(const Vector3 &position)
  {
    _simulationState->_frame.SetPosition(position);
    _renderVisualState[0]->_frame.SetPosition(position);
    _renderVisualState[1]->_frame.SetPosition(position);
  };

  DEFINE_CASTING(PlainSimulation)

} //namaspace