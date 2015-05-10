#pragma once

#ifndef _HELIOS_STAR_TYPE
#define _HELIOS_STAR_TYPE

//===================================================================================================
// included dependencies
#include "../common.h"
#include "ObjectType.h"

namespace Helios
{

//===================================================================================================
// forward declared dependencies


//===================================================================================================
// StarType class


class StarType : public ObjectType
{
typedef ObjectType base;

  private:
    // initialize this class (not virtual, so it can be called from constructor)
  void InitClass();

public:
  StarType(HString name);
  virtual ~StarType();
  virtual void ReInit() override;

  //------------------------------------------------------------------------------
  //! create entity of this type
  virtual Entity *CreateObject(Entity *parent, const HString &name) override;

  USE_CASTING(base)
};

}  //namespace

#endif // _HELIOS_START_TYPE