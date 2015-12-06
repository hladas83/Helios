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


  public:
    StarSystemSimulation();
    virtual ~StarSystemSimulation();
    virtual void InitClass(Entity *simulationOwner, const  WParamItem &simulationCfg) override;

    virtual void Simulate(Entity *simulationParent, float deltaT) override;

    USE_CASTING(base);
    DECL_SIMULATIONTYPE_FACTORY_REG(StarSystemSimulation);
  };

} // Helios namespace

#endif //  _HELIOS_STARSYSTEM_SIMULATION