#pragma once

#ifndef _HELIOS_PLAIN_SIMULATION
#define _HELIOS_PLAIN_SIMULATION

//===================================================================================================
// included dependencies

#include "../../../common.h"
#include <iostream>

//===================================================================================================
// forward declared dependencies

namespace Helios
{

  class Entity;
  class PlainSimulation;

  //===================================================================================================
  // object state representation

  enum __ObejctStateTypes
  {
    RenderState,
    SimulationState
  };

  class ObejctState : public RefCount, public OLinkBase<ObejctState>
  {
  public:

    //! object position and rotation
    Matrix4 _frame;
    float _animationPeriod;
    bool _valid;

    ObejctState() : _animationPeriod(0), _valid(true) {_frame.SetIdentityMatrix();};
  };

  //===================================================================================================


  class PlainSimulation : public RefCount, public OLinkBase<PlainSimulation>
  {
  protected:
    float _timeSimulated;

    // index of current render state, could be static, not yet decided
    int _currentRenderState;
    //! render state, read only, used by all; one is currently used, second is beign prepaired for next frame
    Ref<ObejctState> _renderVisualState[2];
    //! results of current simulation
    Ref<ObejctState> _simulationState;

    //entity using this simualtion
    OLink<Entity> _simulationOwner;
    // simulation confog class
    WParamItem _simulationConfig;

  public:
    PlainSimulation();
    virtual ~PlainSimulation();
    virtual void InitClass(Entity *simulationOwner, const  WParamItem &simulationCfg) ;

    Entity *GetSimulationOwner() {return _simulationOwner.GetObj();};
    const Entity *GetSimulationOwner() const {return _simulationOwner.GetObj();};

    //get simulation state of entity
    __forceinline ObejctState *GetSimulationState() const {return _simulationState;}
    //get render state of entity
    __forceinline ObejctState *GetRenderVisualState() const {return _renderVisualState[_currentRenderState];}

    //by default, do nothing
    virtual void Simulate(Entity *simulationParent, float deltaT) {_timeSimulated += (deltaT);};
    virtual void PostSimulate();
    virtual void PostDraw();

    //!Set position
    void SetPosition(float x, float y, float z);
    void SetPosition(const Vector3 &position);

    //!Get position
    template<__ObejctStateTypes type> 
    Vector3 GetPosition() const { static_assert(false); return VZero;};

    template<> Vector3 GetPosition<RenderState>() const {return _renderVisualState[_currentRenderState]->_frame.GetPosition();};
    template<> Vector3 GetPosition<SimulationState>() const {return _simulationState->_frame.GetPosition();};

    USE_CASTING_ROOT;
  };

  typedef ClassFactory<PlainSimulation, HString> SimulationTypeFactory;
#define DECL_SIMULATIONTYPE_FACTORY_REG(TYPE) static SimulationTypeFactory::RegistrationHelper<TYPE> __factoryHelper__
#define DEF_SIMULATIONTYPE_FACTORY_REG(TYPE, VALUE)  SimulationTypeFactory::RegistrationHelper<TYPE> TYPE::__factoryHelper__(VALUE)


} //namespace

#endif // _HELIOS_PLAIN_SIMULATION