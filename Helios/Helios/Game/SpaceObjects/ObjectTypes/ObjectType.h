#pragma once

#ifndef _HELIOS_ObjectType
#define _HELIOS_ObjectType

//===================================================================================================
// included dependencies
#include "../Common.h"

namespace Helios
{

  //===================================================================================================
  // forward declared dependencies

  class Entity; 

  //===================================================================================================
  // game class

  class ObjectType : public RefCount, public OLinkBase<ObjectType>
  {
  protected:
    HString _name;

  private:
    // initialize this class (not virtual, so it can be called from constructor)
    void InitClass();

  public:
    ObjectType(HString name);
    virtual ~ObjectType();

    //------------------------------------------------------------------------------
    //! create entity of this type
    virtual Entity *CreateObject(Entity *parent, const HString &name); 

    USE_CASTING_ROOT;
  };

} // Helios namespace

#endif // _HELIOS_ObjectType