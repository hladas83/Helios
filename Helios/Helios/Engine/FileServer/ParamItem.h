#pragma once

#ifndef _HELIOS_PARAMITEM
#define _HELIOS_PARAMITEM

//===================================================================================================
// included dependencies

#include "../common.h"
#include <vector>

namespace Helios
{

  //==================================================================================================
  // Param Wraper
  class IParamItem;
  class WParamItem : public RefCount, public OLinkBase<IParamItem>
  {
  private:
    Ref<IParamItem> _item;

  public:
    //------------------------------------------------------------------------------
    // valid
    inline bool IsValid() const;
    inline bool IsScalar() const;
    inline bool IsBool() const; 
    inline bool IsArray() const; 
    inline bool IsString() const; 
    inline bool IsClass() const ;

    WParamItem() {_item = nullptr;}
    WParamItem(IParamItem *item) {_item = item;}
    WParamItem operator >> (const HString &name) const;

    //------------------------------------------------------------------------------
    // GetValue
    template<typename value_Type> value_Type GetValue() const
    {
      static_assert(false, "Not supported data type");
    }
    template<> float GetValue<float>() const
    {
      return GetValue(0.0f);
    }
    template<> bool GetValue<bool>() const
    {
      return GetValue(false);
    }
    template<> HString GetValue<HString>() const
    {
      return GetValue(HString(""));
    }

    //------------------------------------------------------------------------------
    //! get values from basic types
    float GetValue(float defaultValue) const;
    bool GetValue(bool defaultValue) const;
    HString GetValue(const HString &defaultValue) const;

    //! get values from class
    float ReadValue(const HString &name, float defaultValue) const;
    bool ReadValue(const HString &name, bool defaultValue) const;
    HString ReadValue(const HString &name, const HString &defaultValue) const;

    WParamItem ReadArrayValue(int index) const;
    int ArraySize() const;
    IParamItem *GetRawData() const {return _item;};
  };
  //===================================================================================================
  // IParamItem class

  class IParamItem : public RefCount, public OLinkBase<IParamItem>
  {
    friend WParamItem;

  protected:

    //------------------------------------------------------------------------------
    enum IPTTypes
    {
      PTNone,
      PTScalar,
      PTBool,
      PTArray,
      PTString,
      PTClass,
      PTCount
    } _type;
    //------------------------------------------------------------------------------
    ///config value name
    HString _name;

    IParamItem() { _type = PTNone;};
    virtual ~IParamItem() {};

  public:

    //------------------------------------------------------------------------------
    virtual IParamItem *operator >> (const HString &name) {return nullptr;};
    inline HString GetName() const {return  _name;};
    inline IPTTypes GetType() const {return  _type;};

    USE_CASTING_ROOT;
  };

  //===================================================================================================
  // ParamScalar class

  class ParamScalar : public IParamItem
  {
    typedef IParamItem base;

  private:
    float _value;

  public:
    ParamScalar(HString name, float value) { _type = PTScalar; _name = name;  _value = value;};
    virtual ~ParamScalar() {};
    float GetValue() {return _value;};

    USE_CASTING(base)
  };

  //===================================================================================================
  // ParamBool class

  class ParamBool : public IParamItem
  {
    typedef IParamItem base;

  private:
    bool _value;

  public:
    ParamBool(HString name, bool value) { _type = PTScalar; _name = name;  _value = value;};
    virtual ~ParamBool() {};
    bool GetValue() {return _value;};

    USE_CASTING(base)
  };
  //===================================================================================================
  // ParamString class

  class ParamString : public IParamItem
  {
    typedef IParamItem base;

  private:
    HString _value;

  public:
    ParamString(HString name, HString value) { _type = PTString; _name = name; _value = value;};
    virtual ~ParamString() {};
    HString GetValue() {return _value;};

    USE_CASTING(base)
  };

  //===================================================================================================
  // ParamArray class
  class ParamClass;
  class ParamArray : public IParamItem
  {
    typedef IParamItem base;

  private:
    //!list of all galaxies in space
    std::vector<Ref<IParamItem>> _items;

  public:
    ParamArray(HString name) { _type = PTArray; _name = name;};
    virtual ~ParamArray() {};

    ///number of parametrs in this class (not in parent)
    int GetNItems() {return _items.size();}; 

    ///add parametr to class
    void AddItem(IParamItem *item) {_items.push_back(item);};

    //------------------------------------------------------------------------------
    ///number of parametrs in this class (not in parent)
    IParamItem *GetItem(unsigned int index) const 
    { 
      if(index >= 0 && index < _items.size())
        return _items[index];
      else 
        return nullptr;
    }; 

    //------------------------------------------------------------------------------
    //! perform function for each type
    template<class Functor>
    bool ForEachItem(Functor &f)
    {
      for (std::vector<Ref<IParamItem>>::iterator ii = _items.begin(); ii != _items.end(); ++ii)
      {
        if (f((*ii).second))
        {
          return true;
        }
      }
      return false;
    };

    USE_CASTING(base)
  };

  //===================================================================================================
  // ParamClass class

  class ParamClass : public IParamItem
  {
    typedef IParamItem base;

  private:
    /// list of parameters in class
    std::map<HString, Ref<IParamItem>> _params;
    /// class we inherit from
    Ref<ParamClass> _parent;

  public:
    virtual IParamItem *operator >> (const HString &name) override;
    ParamClass(HString name);
    ParamClass(HString name, HString parent);
    virtual ~ParamClass() {};

    ///add parametr to class
    ParamClass  *AddClass(HString name, HString parent);
    ParamScalar *AddScalar(HString name, float value);
    ParamBool   *AddBool(HString name, bool value);
    ParamString *AddString(HString name, HString value);
    ParamArray  *AddArray(HString name);

    //------------------------------------------------------------------------------
    //! perform function for each type
    template<class Functor>
    bool ForEachItem(Functor &f)
    {
      if (_parent)
      {
        if(_parent->ForEachItem(f))
          return true;
      }
      for (std::map<HString, Ref<IParamItem>>::iterator ii = _params.begin(); ii != _params.end(); ++ii)
      {
        if (f((*ii).second))
        {
          return true;
        }
      }
      return false;
    };

    USE_CASTING(base)
  };


} // Helios namespace

#endif // _HELIOS_PARAMITEM