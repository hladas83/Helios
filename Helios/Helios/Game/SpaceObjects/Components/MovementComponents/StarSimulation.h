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

  public:
    StarSimulation();
    virtual ~StarSimulation();
    virtual void InitClass(Entity *simulationOwner, const  WParamItem &simulationCfg) override;

    virtual void Simulate(Entity *simulationParent, float deltaT) override;

    USE_CASTING(base);
    DECL_SIMULATIONTYPE_FACTORY_REG(StarSimulation);
  };

} // Helios namespace

#endif //  _HELIOS_STAR_SIMULATION