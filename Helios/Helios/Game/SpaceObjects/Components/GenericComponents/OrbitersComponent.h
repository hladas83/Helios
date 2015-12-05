#pragma once

//===================================================================================================
// included dependencies

#include "../../../common.h"
#include "../Engine/Components/ComponentHolder.h"
//===================================================================================================
// forward declared dependencies

namespace Helios
{

  class Entity;

  class OrbitersComponent : public ComponentHolder
  {
  private:
    //! orbiters that belongs to star
    std::list<Ref<Entity>> _orbiters;

  public:
    OrbitersComponent(ComponentHolder *parent, WParamItem &entityConfig);
    ~OrbitersComponent();

    //!Simulate loop
    virtual void Simulate(float deltaT) override;
    virtual void PostSimulate() override;
    //! Draw thread method
    virtual void Draw() override;
    virtual void PostDraw() override;

    //------------------------------------------------------------------------------
    //! perform function for each orbiter
    template<class Functor> void ForEachOrbiter(Functor &f)
    {
      std::for_each(_orbiters.begin(), _orbiters.end(), f);
    };
  };

} //namespace
