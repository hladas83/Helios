#pragma once

#ifndef  _HELIOS_STARSYSTEM_SIMULATION
#define  _HELIOS_STARSYSTEM_SIMULATION

//===================================================================================================
// included dependencies

#include "PlainSimulation.h"

namespace Helios
{

//===================================================================================================
// StarSimulation class

  class StarSystemSimulation : public PlainSimulation
  {
    typedef PlainSimulation base;

  private:
    //simulation parametrs
    float _siderealRotationPeriod; ///rotation around its axis of rotation 

  private:
    // initialize this class (not virtual, so it can be called from constructor)
    void InitClass();

  public:
    StarSystemSimulation(Entity *simulationOwner, const  WParamItem &simulationCfg);
    virtual ~StarSystemSimulation();

    virtual void Simulate(Entity *simulationParent, float deltaT) override;

    USE_CASTING(base)
  };

} // Helios namespace

#endif //  _HELIOS_STARSYSTEM_SIMULATION