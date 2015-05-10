//===================================================================================================
// included dependencies
#include "ParamItem.h"
#include "../Engine.h"

namespace Helios
{
  //===================================================================================================

  bool WParamItem::IsValid() const    {return _item != nullptr;};
  bool WParamItem::IsScalar() const   {return _item &&  _item->GetType() == IParamItem::PTScalar;};
  bool WParamItem::IsBool() const     {return _item &&  _item->GetType() == IParamItem::PTBool;};
  bool WParamItem::IsArray() const    {return _item &&  _item->GetType() == IParamItem::PTArray;};
  bool WParamItem::IsString() const   {return _item &&  _item->GetType() == IParamItem::PTString;};
  bool WParamItem::IsClass() const    {return _item &&  _item->GetType() == IParamItem::PTClass;};

  //-----------------------------------------------------------------------------

  WParamItem WParamItem::operator >> (const HString &name) const
  {
    WParamItem wParam = WParamItem(nullptr);
    if(_item && IsClass()) wParam._item = *_item >> name;
    return wParam;
  }

    //-----------------------------------------------------------------------------

  float WParamItem::GetValue(float defaultValue) const
  {
    if(IsScalar())
    {
      ParamScalar *param = dyn_cast<ParamScalar>(_item.GetRef());
      if(param) return param->GetValue();
    }
    return defaultValue;
  }


  //-----------------------------------------------------------------------------

  bool WParamItem::GetValue(bool defaultValue) const
  {
    if(IsBool())
    {
      ParamBool *param = dyn_cast<ParamBool>(_item.GetRef());
      if(param) return param->GetValue();
    }
    return defaultValue;
  }

  //-----------------------------------------------------------------------------

  HString WParamItem::GetValue(const HString &defaultValue) const
  {
    if(IsString())
    {
      ParamString *param = dyn_cast<ParamString>(_item.GetRef());
      if(param) return param->GetValue();
    }
    return defaultValue;
  }

  //-----------------------------------------------------------------------------

  float WParamItem::ReadValue(const HString &name, float defaultValue) const
  {
    if(IsClass())
    {
      IParamItem *iParam = *_item >> name;
      ParamScalar *param = dyn_cast<ParamScalar>(iParam);
      if(param) return param->GetValue();
    }
    return defaultValue;
  }

  //-----------------------------------------------------------------------------

  bool WParamItem::ReadValue(const HString &name, bool defaultValue) const
  {
    if(IsClass())
    {
      IParamItem *iParam = *_item >> name;
      ParamBool *param = dyn_cast<ParamBool>(iParam);
      if(param) return param->GetValue();
    }
    return defaultValue;
  }

  //-----------------------------------------------------------------------------

  HString WParamItem::ReadValue(const HString &name, const HString &defaultValue) const
  {
    if(IsClass())
    {
      IParamItem *iParam = *_item >> name;
      ParamString *param = dyn_cast<ParamString>(iParam);
      if(param) return param->GetValue();
    }
    return defaultValue;
  }

  //-----------------------------------------------------------------------------

  WParamItem WParamItem::ReadArrayValue(int index) const
  {
    WParamItem wParam = WParamItem(nullptr);
    if(IsArray())
    {
      ParamArray *param = dyn_cast<ParamArray>(_item.GetRef());
      if(param) wParam._item = param->GetItem(index);
    }
    return wParam;
  }

  //-----------------------------------------------------------------------------

  int WParamItem::ArraySize() const
  {
    if(IsArray())
    {
      ParamArray *param = dyn_cast<ParamArray>(_item.GetRef());
      if(param) return param->GetNItems();
    }
    return 0;
  }

  //===================================================================================================
  // IParamItem class
  DEFINE_CASTING(IParamItem);
  
  //===================================================================================================
  // ParamScalar class
  DEFINE_CASTING(ParamScalar);
  
  //===================================================================================================
  // ParamScalar class
  DEFINE_CASTING(ParamBool);

  //===================================================================================================
  // ParamArray class
  DEFINE_CASTING(ParamArray);
  
  //===================================================================================================
  // ParamString class
  DEFINE_CASTING(ParamString);
  
  //===================================================================================================
  // ParamClass class
  DEFINE_CASTING(ParamClass);

  ParamClass::ParamClass(HString name)
  { 
    _type = PTClass; _name = name;
  };
  
  //-----------------------------------------------------------------------------

  ParamClass::ParamClass(HString name, HString parent)
  { 
    _type = PTClass; _name = name;

    if(parent.length() > 0) 
    {
      WParamItem wParent = GCoreConfig >> parent;
      if(wParent.IsValid()) _parent = dyn_cast<ParamClass>(wParent.GetRawData());
    }
  };
 

  //-----------------------------------------------------------------------------

  ///add parametr to class
  ParamClass *ParamClass::AddClass(HString name, HString parent)
  {
    _params[name.data()] = new ParamClass(name, parent);
    if(_params.find(name) != _params.end()) 
    {
      return dyn_cast<ParamClass>(_params[name].GetRef());
    }

    return nullptr;
  }

  //-----------------------------------------------------------------------------

  ParamScalar *ParamClass::AddScalar(HString name, float value)
  {
    _params[name.data()] = new ParamScalar(name, value);
    if(_params.find(name) != _params.end()) return dyn_cast<ParamScalar>(_params[name].GetRef());
    return nullptr;
  }

  //-----------------------------------------------------------------------------

  ParamBool *ParamClass::AddBool(HString name, bool value)
  {
    _params[name.data()] = new ParamBool(name, value);
    if(_params.find(name) != _params.end()) return dyn_cast<ParamBool>(_params[name].GetRef());
    return nullptr;
  }

  //-----------------------------------------------------------------------------

  ParamString *ParamClass::AddString(HString name, HString value)
  {
    _params[name.data()] = new ParamString(name,value);
    if(_params.find(name) != _params.end()) return dyn_cast<ParamString>(_params[name].GetRef());
    return nullptr;
  }

  //-----------------------------------------------------------------------------

  ParamArray *ParamClass::AddArray(HString name)
  {
    _params[name.data()] = new ParamArray(name);
    if(_params.find(name) != _params.end()) return dyn_cast<ParamArray>(_params[name].GetRef());
    return nullptr;
  }

  //-----------------------------------------------------------------------------

  IParamItem *ParamClass::operator >> (const HString &name)
  {
    HString copy = name;
    std::transform(copy.begin(), copy.end(), copy.begin(), ::tolower);
    if(_params.find(copy) != _params.end()) return (_params[copy].GetRef());
    else if(_parent) return *_parent >> copy;
    return nullptr;
  };

} // Helios namespace
