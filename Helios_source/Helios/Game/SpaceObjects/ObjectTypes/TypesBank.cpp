//===================================================================================================
// included dependencies
#include "../../../common.h"
#include <iostream>

#include "TypesBank.h"
#include "OrbiterType.h"
#include "StarType.h"
#include "../../Engine/Engine.h"

namespace Helios
{


//===================================================================================================
// object types class


  //------------------------------------------------------------------------------  

  TypesBank::TypesBank() 
  {
  };

  //------------------------------------------------------------------------------  

  TypesBank::~TypesBank() 
  {
  };

  //------------------------------------------------------------------------------  

  void TypesBank::Init()
  {
    //create object templates
    _itemsBank["orbiter"] = new OrbiterType("orbiter");
    _itemsBank["star"] = new StarType("orbiter");
  }

  //------------------------------------------------------------------------------

  //! load new item, if not already present
  bool TypesBank::LoadItem(const char* itemPath)
  {
    HString itemNameLower = HString(itemPath);
    std::transform(itemNameLower.begin(), itemNameLower.end(), itemNameLower.begin(), ::tolower);

    if(!GetItem(itemPath))
    {
      if(strcmpi("orbiter", itemNameLower.data()) == 0) _itemsBank[itemNameLower.data()] = new OrbiterType(itemNameLower);
      else if(strcmpi("star", itemNameLower.data()) == 0) _itemsBank[itemNameLower.data()] = new StarType(itemNameLower);
      else Assert(false);
    }
    return true;
  }

  //------------------------------------------------------------------------------

  Entity *TypesBank::CreateObject(Entity *parent, const HString &name)
  {
    WParamItem entityCfg = GCoreConfig >> name;
    WParamItem typeItem = entityCfg >> "type";

    if(typeItem.IsString())
    {
      HString typeName = typeItem.GetValue(HString(""));
      if(_itemsBank.find(typeName) != _itemsBank.end()) return _itemsBank[typeName]->CreateObject(parent, name);
    }
    return nullptr;
  }


} //namespace