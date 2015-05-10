#pragma once

#ifndef _HELIOS_ORBITER_SIMULATION
#define _HELIOS_ORBITER_SIMULATION

//===================================================================================================
// included dependencies

#include "PlainSimulation.h"
class WParamItem;

namespace Helios
{

//===================================================================================================
// OrbiterSimulation class


  class OrbiterSimulation : public PlainSimulation
  {
    typedef PlainSimulation base;

  private:
    //simulation parametrs
    float _siderealRotationPeriod; ///rotation around its axis of rotation 
    float _orbitalPeriod; ///rotation around its axis of rotation 
    float _inclination;

    float _aphelion; /// biggest distance from gravity parent
    float _perihelion; /// smallest distance from gravity parent
    float _ellipseA;
    float _ellipseB;
    float _excentricity;

    OLink<Entity> _gravityParent; // entity we orbit around

  private:
    // initialize this class (not virtual, so it can be called from constructor)
    void InitClass();

  public:
    OrbiterSimulation(Entity *simulationOwner, const  WParamItem &simulationCfg);
    virtual ~OrbiterSimulation();
    virtual void ReInit() override;

    //------------------------------------------------------------------------------
    __forceinline float GetEllipseA() const  {return _ellipseA;};
    __forceinline float GetEllipseB() const  {return _ellipseB;};
    __forceinline float GetExcentricity() const  {return _excentricity;};
    __forceinline float GetSiderealRotationPeriod() const  {return _siderealRotationPeriod;};
    __forceinline float GetOrbitalPeriod() const  {return _orbitalPeriod;};
    __forceinline float GetInclination() const  {return _inclination;};

    //------------------------------------------------------------------------------
    const Entity *GetGravityParent() const; 
    void SetGravityParent(Entity *gravityParent);

    virtual void Simulate(Entity *simulationParent, float deltaT) override;

    USE_CASTING(base)
  };

} // Helios namespace

#endif // _HELIOS_ORBITER_SIMULATION