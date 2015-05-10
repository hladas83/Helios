#pragma once

#ifndef _HELIOS_ORBITER_TYPE
#define _HELIOS_ORBITER_TYPE


//===================================================================================================
// included dependencies
#include "../common.h"
#include "ObjectType.h"

namespace Helios
{

//===================================================================================================
// forward declared dependencies

  class RenderObject;

//===================================================================================================
// game class


  class OrbiterType : public ObjectType
  {
    typedef ObjectType base;

  private:

    //! object shape description
    Ref<RenderObject> _renderOrbitObject;

  private:
    // initialize this class (not virtual, so it can be called from constructor)
    void InitClass();

  public:
    OrbiterType(HString name);
    virtual ~OrbiterType();
    virtual void ReInit() override;

    //------------------------------------------------------------------------------
    //! get object shape description
    inline Ref<RenderObject> GetRenderOrbitObject() const {return _renderOrbitObject;};

    //------------------------------------------------------------------------------
    //! create entity of this type
    virtual Entity *CreateObject(Entity *parent, const HString &name) override;

    USE_CASTING(base)
  };

} // Helios namespace

#endif // _HELIOS_ORBITER_TYPE