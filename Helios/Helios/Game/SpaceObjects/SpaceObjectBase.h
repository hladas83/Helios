#pragma once

#ifndef _HELIOS_SpaceObjectBase
#define _HELIOS_SpaceObjectBase

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

  class SpaceObjectBase : public Entity
  {
    typedef Entity base;

  protected:
    //! object shape/shader/texture description
    Ref<RenderObject> _iconRenderObject;
    HColor _infoColor;

  private:
    // initialize this class (not virtual, so it can be called from constructor)
    void InitClass();

  public:
    SpaceObjectBase(Entity *parent, ObjectType *type, const HString &name);
    virtual ~SpaceObjectBase();
    virtual void ReInit() override;

    //------------------------------------------------------------------------------
    //!Simulate loop
    virtual void Simulate(float deltaT) override;
    virtual void PostSimulate() override;

    //------------------------------------------------------------------------------
    //! Draw thread method
    virtual void Draw() override;
    virtual void PostDraw() override;

    USE_CASTING(base)
  };

} // Helios namespace

#endif // _HELIOS_SpaceObjectBase