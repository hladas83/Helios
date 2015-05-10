#pragma once

#ifndef _HELIOS_REFCOUNTER_TYPE
#define _HELIOS_REFCOUNTER_TYPE

#define REF_MT_SAFE 1

//needed for InterlockedIncrementAcquire
#include <Windows.h>

//! This class as well as Ref template class are designed to provide smart pointer operations.
/*!
  Using this classes you need not take care of releasing structures you have allocated.
  There is a reference count attribute inside RefCount class (_count) which helps us to determine wheter there still exists some valid pointer to it.
  In case there is not we will delete whole object without fear.

  Note that at the end of the scope where pMO* is defined, the object will be automatically released.
  Another note that you MUST use the new keyword when allocating the space for the object.
  Otherwisely constructor would not be called and _count would not be initialized.
  \sa Ref
*/

//===================================================================================================
template <bool mtSafe>
class RefCountBaseT
{
private:
  //! Number of references to this object.
  mutable int _count;

public:
  //! Default constructor.
  RefCountBaseT(){_count=0;}
  //! Copy constructor (_count attribute zeroed).
  /*!
    \param src Reference to source object.
  */
  RefCountBaseT( const RefCountBaseT &src ){_count=0;}
  //! Copying of object (_count attribute of the target is left intact - target already exists).
  void operator =( const RefCountBaseT &src ){}
  //! Destructor
public:
  //! Adding of reference to this object.
  /*!
    \return Number of references.
  */
  __forceinline int AddRef() const
  {
#if CHECK_REFCOUNT_MT
    AssertMainThread();
#endif
    return ++_count;
  }
  //! Determines number of references to this object.
  /*!
    \return Number of references.
  */

  //! Releasing of reference to this object.
  /*!
    \return Number of references.
  */
  __forceinline int DecRef() const
  {
#if CHECK_REFCOUNT_MT
    AssertMainThread();
#endif
    int ret =--_count;
    return ret;
  }
  /// check reference count
  __forceinline int RefCounter() const {return _count;}
};

//===================================================================================================
/// specialize the MT safe variant
template <>
class RefCountBaseT<true>
{
private:
  //! Number of references to this object.
  volatile mutable long _count;

public:
  //! Default constructor.
  RefCountBaseT(){_count=0;}
  //! Copy constructor (_count attribute zeroed).
  /*!
    \param src Reference to source object.
  */
  RefCountBaseT( const RefCountBaseT &src ){_count=0;}
  //! Copying of object (_count attribute of the target is left intact - target already exists).
  void operator = ( const RefCountBaseT &src ){}
public:
  //! Adding of reference to this object.
  /*!
    \return Number of references.
  */
	__forceinline int AddRef() const {return  InterlockedIncrementAcquire(&_count);}

  //! Releasing of reference to this object.
  /*!
    \return Number of references.
  */
  __forceinline int DecRef() const
  {
    int ret = InterlockedDecrementRelease(&_count); //flush all writes
    return ret;
  }
  /// Determines number of references to this object.
  /*!
    \return Number of references.
  */
  __forceinline int RefCounter() const {return _count;}
};

#if REF_MT_SAFE
const bool RefMTSafe = true;
#else
const bool RefMTSafe = false;
#endif


//! This class as well as Ref template class are designed to provide smart pointer operations.
/*!
This class is potentially unsafe, as there are no virtual functions used.
When using it, you need to make sure pointer to relevant child is always used.
*/

//===================================================================================================
class RefCountBase: public RefCountBaseT<RefMTSafe>
{
};

//===================================================================================================
//! This class as well as Ref template class are designed to provide smart pointer operations.
/*!
Using this classes you need not take care of releasing structures you have allocated.
There is a reference count attribute inside RefCount class (_count) which helps us to determine whether there still exists some valid pointer to it.
In case there is not we will delete whole object without fear.

Note that at the end of the scope where pMO* is defined, the object will be automatically released.
Another note that you MUST use the new keyword when allocating the space for the object.
Otherwisely constructor would not be called and _count would not be initialized.
\sa Ref
*/
class RefCount: public RefCountBaseT<RefMTSafe>
{
	typedef RefCountBaseT<RefMTSafe> base;
public:
	virtual ~RefCount(){} // use destructor of derived class
	//! Adding of reference to this object.
	/*!
	\return Number of references.
	*/
#if _DEBUG
	/// assert/debugging opportunity - verify object state is correct when used
	/**
	Debugging only - Never called in release build.
	*/
	virtual void OnUsed() const {}
	__forceinline int AddRef() const
	{
		if (RefCounter()==0) OnUsed();
		return base::AddRef();
	}
#endif
	//! Determines number of references to this object.
	/*!
	\return Number of references.
	*/

	//! Releasing of reference to this object.
	/*!
	\return Number of references.
	*/
	__forceinline int ReleaseRef() const
	{
		int ret = base::DecRef();
		if( ret==0 ) OnUnused();
		return ret;
	}
	/// destroy an object using delete
	void Destroy() const
	{
		delete const_cast<RefCount *>(this);
	}
	/// called when object is no longer used and may be released
	/**
	Default implementation calls delete
	Typical override may perform some caching
	*/
	virtual void OnUnused() const {Destroy();}
	/// get memory used by this object
	/**
	Does not include memory used to hold this object itself,
	which is already covered by sizeof
	*/
	virtual double GetMemoryUsed() const {return 0;}

protected:
	//@{ pretend the shape is still loaded during its unloading to avoid destructor recursion
	void TempAddRef() {base::AddRef();}
	void TempRelease() {base::DecRef();}
	//@}
};

//===================================================================================================
//! This template class provides a smart pointers.
/*!
This template class provides a reference substitution for smart pointer operations.
Type must have members AddRef and Release defined. This can be accomplished
by deriving from RefCount class.
\sa RefCount, SRef, ComRef
*/
template<class Type>
class Ref
{
protected:
	//! Reference to the object.
	Type *_ref;
public:
	//! make data type accessible
	typedef Type ItemType;
	//! Default constructor.
	__forceinline Ref(){_ref=nullptr;}
	// Using this constructor you can get the existing reference from source parameter.
	/*!
	\param source Source pointer to an object.
	*/
	Ref( Type *source )
	{
		if( source ) source->AddRef();
		_ref=source;
	}
	//! Copying of pointer.
	const Ref<Type> &operator = ( Type *source )
	{
		Type *old = _ref; 
		if( source ) source->AddRef();
		// for MT safety of shared Ref objects we need to stop pointing to the object before we release the last ref to it
		_ref=source;
		if( old ) old->ReleaseRef();
		return *this;
	}
	// Copy constructor.
	/*!
	\param sRef Reference to the source pointer.
	*/
	Ref( const Ref &sRef )
	{
		Type *source=sRef._ref;
		if( source ) source->AddRef();
		_ref=source;
	}
	//! Copying of pointer.
	const Ref<Type> &operator = ( const Ref &sRef )
	{
		Type *source = sRef._ref;
		Type *old = _ref; 
		if( source ) source->AddRef();
		// for MT safety of shared Ref objects we need to stop pointing to the object before we release the last ref to it
		_ref=source;
		if( old ) old->ReleaseRef(); // Release will do Publish as well (part of InterlockedDecrementRelease)
		return *this;
	}
	//! Determining whether pointer is not null.
	/*!
	\return TRUE in case pointer is not null, FALSE otherwise.
	*/
	__forceinline bool NotNull() const {return _ref!=nullptr;}
	//! Determining whether pointer is null.
	/*!
	\return TRUE in case pointer is null, FALSE otherwise.
	*/
	__forceinline bool IsNull() const {return _ref==nullptr;}
	//! Destructor - heart of automatic releasing.
	__forceinline ~Ref(){Free();}
	//! This method will release the object (or at least decrements it's reference counter).
	/*!
	However you are not forced to use it. Object will release itself automatically.
	*/
	void Free(){if( _ref ) _ref->ReleaseRef(),_ref=nullptr;}
	//! This method will return standard pointer to the object.
	/*!
	\return Standard pointer to the object.
	*/
	__forceinline Type *GetRef() const {return _ref;}
	//! This method will set reference to the object.
	/*!
	You must use this function with infernal caution. For instance if you set it carelessly to nullptr,
	you can be sure that the previous object will never be deleted.
	\param ref New pointer to the object.
	*/
	__forceinline void SetRef( Type *ref ) {_ref=ref;}
	//! This operator is suitable for accessing object members. It has the same meaning like standard -> operator.
	__forceinline Type *operator -> () const {return _ref;}
	//! Operator for type casting.
	__forceinline operator Type *() const {return _ref;}

	//! calculate memory used
	double GetMemoryUsed() const
	{
		// for each owner only appropriate part of shared object is considered
		if (!_ref) return 0;
		return double(sizeof(Type)+_ref->GetMemoryUsed())/_ref->RefCounter();
	}
};


#endif // _HELIOS_REF_COUNTER