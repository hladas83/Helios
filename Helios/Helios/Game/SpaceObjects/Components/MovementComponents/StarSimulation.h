#pragma once

#ifndef  _HELIOS_STAR_SIMULATION
#define  _HELIOS_STAR_SIMULATION

//===================================================================================================
// included dependencies

#include "PlainSimulation.h"

namespace Helios
{

//===================================================================================================
// StarSimulation class

  class StarSimulation : public PlainSimulation
  {
    typedef PlainSimulation base;

  private:
    //simulation parametrs
    float _siderealRotationPeriod; ///rotation around its axis of rotation 

  private:
    // initialize this class (not virtual, so it can be called from constructor)
    void InitClass();

  public:
    StarSimulation(Entity *simulationOwner, const  WParamItem &simulationCfg);
    virtual ~StarSimulation();

    virtual void Simulate(Entity *simulationParent, float deltaT) override;

    USE_CASTING(base)
  };

} // Helios namespace

#endif //  _HELIOS_STAR_SIMULATION