#pragma once

#ifndef _TYPES
#define _TYPES

#include <iostream>
#include "SmartPointers/Ref.h"
#include "SmartPointers/SoftLink.h"
#include "SmartPointers/InitPtr.h"
#include "SmartPointers/SRef.h"
#include "dynCast.h"
#include "Matrix4.h"
#include "Singleton.h"
#include "color.h"


#include <cstdint>

#define OLink(Type) SoftLink<Type, SoftLinkTraitsLocked<Type> >

#define HString  std::string

//-----------------------------------------------------------------------------
// often used vectors
#define  VZero    Vector3(0,0,0)
#define  VFront   Vector3(0,0,1)
#define  VUp      Vector3(0,1,0)
#define  VSide    Vector3(1,0,0)

//-----------------------------------------------------------------------------
//constatnts
#define PI     1.57079632f
#define PI_H   3.14159265f
#define PI_2   6.28318531f

#define RAD2DEG(XX)  ((XX/PI_H)*180)
#define DEG2RAD(XX)  ((XX/180)*PI_H)  

#endif // _TYPES