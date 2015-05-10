#pragma once

#ifndef _HELIOS_POINTER_TYPE
#define _HELIOS_POINTER_TYPE

#include <exception>
#include <iostream>

//===================================================================================================
//object with ref counter
template<class Type> class _ObjectWRC
{
 private:
  unsigned int _referenceCount; // ref counter
  Type *_data; // data pointer

  // never to be called
  _ObjectWRC(const _ObjectWRC<Type>&);
  _ObjectWRC &operator=(const _ObjectWRC<Type>&);

 public:
  _ObjectWRC(Type *normalPointer)
    : _referenceCount(0), _data(normalPointer) {}
  virtual ~_ObjectWRC() { delete _data; }

  // data manipulation
  const Type *getData() const { return _data; }
  Type *getData() { return _data; }
  void setData(Type *normalPointer) { _data = normalPointer; }

  // counter manipulation
  unsigned int getReferenceCount() const { return _referenceCount; }
  unsigned int increment() { return ++_referenceCount; }
  unsigned int decrement() { return --_referenceCount; }
};

//===================================================================================================
template<class Type> class Pointer
{
 protected:
  _ObjectWRC<Type> *_object; // object with counter
 public:
  // contructor
  Pointer() : _object(nullptr) {}

  Pointer(const Pointer<Type> &pointer): _object(pointer._object)
  {
    if (_object != nullptr)
    {
      _object->increment();
    }
  };

  Pointer(Type *normalPointer)
  {
    _object = new _ObjectWRC<Type>(normalPointer);
    if (_object != nullptr)
    {
      _object->increment();
    }
  };

  // destructor
  ~Pointer()
  { 
    if ( (_object != nullptr) && (_object->decrement() == 0) )
    {
      delete _object;
    }
    _object = nullptr;
  }

  // operator =
  Pointer<Type> &operator=(const Pointer<Type> &pointer)
  {
    if(_object)
    {
      if ( (_object != nullptr) && (_object->decrement() == 0) )
      {
        delete _object;
      }
    }
    _object = pointer._object;
    _object->increment();
    return *this;
  };

  Pointer<Type> &operator=(Type *normalPointer) 
  {
    if(_object)
    {
      if ( (_object != nullptr) && (_object->decrement() == 0) )
      {
        delete _object;
      }
    }
    _object = new _ObjectWRC<Type>(normalPointer);
    _object->increment();
    return *this;
  };
  
  // operator ==, !=
  bool operator==(const Pointer<Type> &pointer) 
  { 
    return this->_object == pointer._object;
  }

  bool operator==(Type *normalPointer)
  {
    return this->_object->getData() == normalPointer;
  };

  bool operator==(const Type *normalPointer) const
  {
    return this->_object->getData() == normalPointer;
  };

  bool operator!=(const Pointer<Type> &pointer) 
  { 
    return !(this->_object == pointer._object);
  }

  bool operator!=(Type *normalPointer)
  {
    return !(this->_object->getData() == normalPointer);
  };

  bool operator!=(const Type *normalPointer) const
  {
    return !(this->_object->getData() == normalPointer);
  };

  const Type &operator*() const //throw (std::runtime_error)
  {
    if (_object == nullptr)
    {
      //throw std::runtime_error("Null pointer is dereferenced");
    }
    register Type *ret = _object->getData();
    if (ret == nullptr)
    {
     // throw std::runtime_error("Null pointer is dereferenced");
    }
    return *ret;
  }

   // operator *, ->
  Type &operator*() //throw (std::runtime_error)
  {
    if (_object == nullptr)
    {
      //throw std::runtime_error("Null pointer is dereferenced");
    }
    register Type *ret = _object->getData();
    if (ret == nullptr)
    {
     // throw std::runtime_error("Null pointer is dereferenced");
    }
    return *ret;
  }

  // operator -> 
  const Type *operator->() const //throw (std::runtime_error)
  {
    if (_object == nullptr)
    {
      //throw std::runtime_error("Null pointer is dereferenced");
    }
    register Type *ret = _object->getData();
    if (ret == nullptr)
    {
      //throw std::runtime_error("Null pointer is dereferenced");
    }
    return *ret;
  }

  Type *operator->() //throw (std::runtime_error)
  {
    if (_object == nullptr)
    {
   //   throw std::runtime_error("Null pointer is dereferenced");
    }
    register Type *ret = _object->getData();
    if (ret == nullptr)
    {
    //  throw std::runtime_error("Null pointer is dereferenced");
    }
    return ret;
  }

  // number of references
  unsigned int getReferenceCount() const 
  { 
    return _object == nullptr ? 0 : _object->getReferenceCount(); 
  }

  // set object to null
  void setnullptr() 
  {
    if(_object)
    {
      if ( (_object != nullptr) && (_object->decrement() == 0) )
      {
        delete _object;
      }
      _object = nullptr;
    }
  };
  
  // true if obejct is null
  bool isnullptr() { return _object == nullptr; }

  // cast
  template<class NewType> Pointer<NewType> cast() 
    // const throw(std::bad_cast)
  {
    if (_object == nullptr)
    {
      return Pointer<NewType>(static_cast<NewType*>(nullptr));
    }
    if (dynamic_cast<NewType*> (_object->getData()) == nullptr)
    {
     // throw std::bad_cast();
    }
    return Pointer<NewType>((_ObjectWRC<NewType>*)(_object));
  }
};

#endif // _HELIOS_POINTER_TYPE