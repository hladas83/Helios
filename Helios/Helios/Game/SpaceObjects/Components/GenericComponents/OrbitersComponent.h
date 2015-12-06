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
    typedef ComponentHolder base;

  private:
    //! orbiters that belongs to star
    std::list<Ref<Entity>> _orbiters;

  public:
    OrbitersComponent();
    ~OrbitersComponent();
    virtual void InitClass(ComponentHolder *parent, WParamItem &entityConfig) override;

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

    USE_CASTING(base);
    DECL_COMPONENTHOLDER_FACTORY_REG(OrbitersComponent);
  };

} //namespace
