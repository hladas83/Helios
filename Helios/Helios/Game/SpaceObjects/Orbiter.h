#pragma once

#ifndef _HELIOS_ORBITER
#define _HELIOS_ORBITER

//===================================================================================================
// included dependencies

#include "../../common.h"
#include "../Entity.h"

namespace Helios
{

  //===================================================================================================
  // forward declared dependencies

  class ObjectType;
  class OrbiterType;

  //===================================================================================================
  //orbiter object

  class Orbiter : public Entity
  {
    typedef Entity base;

  private:
    //! orbiters that belongs to orbiter (moons, stations,....)
    std::list<Ref<Entity>> _orbiters;

  private:
    // initialize this class (not virtual, so it can be called from constructor)
    void InitClass();

  public:
    Orbiter(ObjectType *type, Entity *parent, const HString &name);
    virtual ~Orbiter();
    virtual void ReInit() override;

    //return real type
    const OrbiterType *Type() const;

    //------------------------------------------------------------------------------
    //!Simulate loop
    virtual void Simulate(float deltaT) override;
    virtual void PostSimulate() override;

    //------------------------------------------------------------------------------
    //! Draw thread method
    virtual void Draw() override;
    virtual void PostDraw() override;

    //------------------------------------------------------------------------------
    //! perform function for each orbiter
    template<class Functor> void ForEachOrbiter(Functor &f)
    {
      std::for_each(_orbiters.begin(), _orbiters.end(), f);
    };


    USE_CASTING(base)
  };

} // Helios namespace

#endif // _HELIOS_ORBITER