#pragma once

#ifndef _HELIOS_STARSYSTEM
#define _HELIOS_STARSYSTEM


//===================================================================================================
// included dependencies

#include "../../Elementary/Types.h"
#include "../Entity.h"

// External libs.
#include <list>

namespace Helios
{

  //===================================================================================================
  // forward declared dependencies

  class Galaxy;

  //===================================================================================================
  // class StarSystem


  class StarSystem : public Entity, public OLinkBase<StarSystem>
  {
    typedef Entity base;

  private:
    //------------------------------------------------------------------------------
    enum EntityTypesEnum
    {
      // objects that belongs to system
      ETObjects,
      // stars that belongs to system
      ETStars,
      // planets that belongs to system
      ETPlanets,
      // objects, that can leave system
      ETShips,
      // types count - must be last
      ETCount
    };
    //------------------------------------------------------------------------------

    //! Objects that belongs to system
    std::list<Ref<Entity>> _objects[ETCount];

  private:
    // initialize this class (not virtual, so it can be called from constructor)
    void InitClass();

  public:
    StarSystem();
    StarSystem(Entity *parent, ObjectType *type, const HString &name);
    virtual void ReInit() override;

    virtual ~StarSystem();

    //------------------------------------------------------------------------------
    //!Simulate loop
    virtual void Simulate(float deltaT) override;
    virtual void PostSimulate() override;

    //------------------------------------------------------------------------------
    //! Draw thread method
    virtual void Draw() override;
    virtual void PostDraw() override;

    //------------------------------------------------------------------------------
    // fidn entity with given config name
    Entity *FindEntity(const HString &name) const;

    //!delete all objects
    void CleanUp();

    USE_CASTING(base)

    //------------------------------------------------------------------------------
    //! perform function for each type
    template<class Functor> void ForEachEntity(Functor &f)
    {
      for (int i = 0; i< ETCount; i++)
      {
        std::for_each(_objects[i].begin(), _objects[i].end(), f);
      }
    };

    //------------------------------------------------------------------------------
    //! perform function for each type
    template<class Functor> void ForEachEntity(Functor &f) const
    {
      for (int i = 0; i< ETCount; i++)
      {
        std::for_each(_objects[i].begin(), _objects[i].end(), f);
      }
    };

  };

} // Helios namespace

#endif // _HELIOS_STARSYSTEM