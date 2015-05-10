#pragma once

#ifndef _HELIOS_SOFTLINK_TYPE
#define _HELIOS_SOFTLINK_TYPE

#include "Ref.h"
#include "Assert.h"

//===================================================================================================
//!  helper class used as common storage place between OLinks and object itself
template<class T>
class OLinkTracker : public RefCount
{
public:
  typedef T ItemType;

private:
  //! Pointer to real object that is traced by this tracer
  mutable ItemType *_objectLink;

public:
  OLinkTracker() : _objectLink(nullptr){};
  OLinkTracker(ItemType *obj) : _objectLink(obj){};

  __forceinline void SetLink(ItemType *obj) { _objectLink = obj; }
  __forceinline void InvalidateLink() { _objectLink = nullptr; }
  __forceinline ItemType *GetLink() { return _objectLink; }

  ~OLinkTracker(){};
};

//===================================================================================================
template<class T>
class OLinkBase
{
public:
  typedef T ItemType;
  typedef OLinkTracker<T> TrackerType;

private:
  mutable Ref<TrackerType> _tracker;

public:
  //! Constructor creates automatically tracker for us and sets his link to proper place.
  OLinkBase(void) : _tracker(new TrackerType()) { _tracker->SetLink(static_cast<T*>(this)); }
  //! Creates new tracker because we don't want to keep same tracker across different instances of one class.
  OLinkBase(const OLinkBase &val) : _tracker(new TrackerType()) { _tracker->SetLink(static_cast<T*>(this)); }
  //! Destructor invalidates link to object inside tracer to let everyone know that object is gone.
  /**
  This is called whenever original object is deleted so we want to invalidate common link.
  This will result in setting NULL to each OLink previously pointing to this object.
  */
  ~OLinkBase() { _tracker->InvalidateLink();};

  __forceinline TrackerType *GetTracker() const { return _tracker; }

  //! Do nothing because when this can be called tracker is already initialized and linked to this object. We don't want to replace him. (always must point to this)
  void operator=(const OLinkBase &op) { }

};

//===================================================================================================
template<class T, class Base = T>
class OLink
{
public:
  typedef OLinkBase<Base> LinkType;

private:
  //! Common (cloud if you wish) space of all OLinks that is pointing to same object instance.
  mutable Ref<typename LinkType::TrackerType> _tracker;


public:
  OLink(void) {}
  //! Allows construction using plain pointer to object
  OLink(LinkType *val) { _tracker = val ? val->GetTracker() : nullptr; }
  //! Allows construction by making copy of existing OLink
  OLink(const OLink<T, Base> &val) { _tracker = val._tracker; }
  //! We don't need to do any operation because only variable tracker is ref counted.
  virtual ~OLink(void) {}

  //! Returns Linked object or NULL if object is deleted or link is not initialized yet.
  __forceinline T *GetObj() { return _tracker ? static_cast<T*>(_tracker->GetLink()) : NULL; }
  __forceinline const T *GetObj() const { return _tracker ? static_cast<T*>(_tracker->GetLink()) : NULL; }

  __forceinline operator T*() { return GetObj(); }
  __forceinline operator const T*() const { return GetObj(); }

  //! Returns Linked object or NULL if object is deleted or link is not initialized yet.
  /**
  Also when linked object is NULL this will raise assert for easier debugging.
  */
  __forceinline T *operator->() { Assert(GetObj()); return GetObj(); }
  __forceinline const T *operator->() const { Assert(GetObj()); return GetObj(); }

  //! Stores new pointer, old one stays unchanged. Changes only tracker address.
  OLink<T, Base> &operator=(LinkType *op) { _tracker = op ? op->GetTracker() : nullptr; return *this; }
  //! Stores another OLink, old one stays unchanged. Changes only tracker address.
  OLink<T, Base> &operator=(const OLink<T, Base> &op) { _tracker = op._tracker; return *this; }
};



#endif //_HELIOS_SOFTLINK_TYPE