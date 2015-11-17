#pragma once

#ifndef _HELIOS_ItemsBankBase
#define _HELIOS_ItemsBankBase

//===================================================================================================
// included dependencies

#include "../../common.h"
#include <algorithm>

namespace Helios
{
  //===================================================================================================
  // general items bank class
 
  template<class Type>
  class ItemsBankBase : public RefCount
  {

  protected:
    std::map<HString, Ref<Type>> _itemsBank;

    ItemsBankBase(){};
    virtual ~ItemsBankBase(){};

  public:
    virtual void Init() = 0;
    virtual void CleanUp() {_itemsBank.clear();};

    //------------------------------------------------------------------------------  
    //! call InitDX11 for each item
    void InitDX11(ID3D11Device* _g_pd3dDevice) 
    {
      ForEachItem([_g_pd3dDevice](Type *item) ->bool {item->InitDX11(_g_pd3dDevice); return false; });
    }

    //------------------------------------------------------------------------------  
    //! load new item, if not already present
    virtual bool LoadItem(const char* itemPath)
    {
      HString itemNameLower = HString(itemPath);
      std::transform(itemNameLower.begin(), itemNameLower.end(), itemNameLower.begin(), ::tolower);

      if(!GetItem(itemNameLower.data())) _itemsBank[itemNameLower.data()] = new Type(itemNameLower.data());
      return true;
    }
    
    //------------------------------------------------------------------------------  
    //! return item with given name, or nullptr if not present
    Type *GetItem( const char* itemName)
    {
      HString itemNameLower = HString(itemName);
      std::transform(itemNameLower.begin(), itemNameLower.end(), itemNameLower.begin(), ::tolower);

      if(_itemsBank.find(itemNameLower.data()) != _itemsBank.end()) return _itemsBank[itemNameLower.data()];
      return nullptr;
    };

    //------------------------------------------------------------------------------  
    //! return item with given name, or nullptr if not present
    Type *GetItem( const HString &itemName)
    {
      return GetItem(itemName.data());
    };

    //------------------------------------------------------------------------------  
    //! return item with given name, or nullptr if not present, if not present load it
    Type *GetLoadItem( const char* itemName)
    {
      Type *item = GetItem(itemName);
      if(item) return item;
      else LoadItem(itemName);
      return GetItem(itemName);
    };

    //------------------------------------------------------------------------------  
    //! return item with given name, or if not present load it
    Type *GetLoadItem( const HString &itemName)
    {
      return GetLoadItem(itemName.data());
    };

    //------------------------------------------------------------------------------  
    //! perform function for each type
    template<class Functor>
    bool ForEachItem(Functor &f)
    {
      for (std::map<HString, Ref<Type>>::iterator ii = _itemsBank.begin(); ii != _itemsBank.end(); ++ii)
      {
        if (f((*ii).second))
        {
          return true;
        }
      }
      return false;
    };
  };


  //===================================================================================================
  // items bank class expanded with InitDX11

  template<class Type>
  class ItemsBankBaseDX11 : public ItemsBankBase<Type>
  {

  public:

    //------------------------------------------------------------------------------  
    //! call InitDX11 for each item
    void InitDX11(ID3D11Device* _g_pd3dDevice) 
    {
      ForEachItem([_g_pd3dDevice](Type *item) ->bool {item->InitDX11(_g_pd3dDevice); return false; });
    }

  };

} // Helios namespace

#endif //_HELIOS_ItemsBankBase