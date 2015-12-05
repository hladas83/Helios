#pragma once

#ifndef _HELIOS_GALAXY
#define _HELIOS_GALAXY

//===================================================================================================
// included dependencies

#include "../../Elementary/Types.h"
#include "../Entity.h"

// External libs.
#include <list>

//game classes

namespace Helios
{

  //===================================================================================================
  // forward declared dependencies

  class StarSystem;
  class Space;


  //===================================================================================================
  // Galaxy class

  class Galaxy : public Entity
  {
    typedef Entity base;

  private:
    OLink<Space> _parent;
    OLink<StarSystem> _activeStarSystem;
    std::list<Ref<StarSystem>> _starSystems;

    // ships moving beetwen starSystems
    std::list<Ref<Entity>> _ships;

  private:
    // initialize this class (not virtual, so it can be called from constructor)
    void InitClass();

  public:
    Galaxy();
    Galaxy(Entity *parent, ObjectType *type, const HString &name);

    virtual ~Galaxy();

    void SetActiveStarSystem(const Ref<StarSystem> starSystem);
    inline bool IsActiveStarSystem() const;

    //------------------------------------------------------------------------------
    //!Simulate loop
    virtual void Simulate(float deltaT) override;
    virtual void PostSimulate() override;

    //------------------------------------------------------------------------------
    //! Draw thread method
    virtual void Draw() override;
    virtual void PostDraw() override;

    //!delete all star systems 
    void CleanUp();

    //! perform function for each type
    template<class Functor> void ForEachStarSystem(Functor &f)
    {
      std::for_each(_starSystems.begin(), _starSystems.end(), f);
    };

    USE_CASTING(base)
  };

} // Helios namespace

#endif // _HELIOS_GALAXY