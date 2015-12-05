#pragma once

#ifndef _HELIOS_STAR
#define _HELIOS_STAR

//===================================================================================================
// included dependencies

#include "../../common.h"
#include "../Entity.h"

//===================================================================================================
// forward declared dependencies

namespace Helios
{
  class ObjectType;
  class StarType;

//===================================================================================================
//star object

  class Star : public Entity
  {
    typedef Entity base;

  private:
    //! orbiters that belongs to star
    std::list<Ref<Entity>> _orbiters;

  private:
    // initialize this class (not virtual, so it can be called from constructor)
    void InitClass();

  public:
    Star(ObjectType *type, Entity *parent, const HString &name);
    virtual ~Star();
    virtual void ReInit() override;

    //return real type
    const StarType *Type() const;

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

#endif // _HELIOS_STAR