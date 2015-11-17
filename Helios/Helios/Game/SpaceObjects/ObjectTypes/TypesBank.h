#pragma once

#ifndef _HELIOS_TYPESBANK
#define _HELIOS_TYPESBANK

//===================================================================================================
// included dependencies
#include "../../../common.h"
#include "../../Elementary/ItemsBankBase/ItemsBankBase.h"
#include "ObjectType.h"

namespace Helios
{

  //===================================================================================================
  // forward declared dependencies

  class ObjectType;
  class Entity;

  //===================================================================================================
  // game class


  class TypesBank : public Singleton<TypesBank>,  public ItemsBankBase<ObjectType>
  {
    typedef ItemsBankBase base;

  public:
    TypesBank();
    virtual ~TypesBank();
    
    virtual void Init() override;
    void InitTypes(); 

    //create new game object based on item template
    Entity *CreateObject(Entity *parent, const HString &name);

    //! load new item, if not already present
    virtual bool LoadItem(const char* itemPath);
  };

} // Helios namespace

#endif // _HELIOS_TYPESBANK