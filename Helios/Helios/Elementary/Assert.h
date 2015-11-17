#pragma once

#ifndef _HELIOS_ASSERT_H
#define _HELIOS_ASSERT_H

#include <assert.h>

#if _DEBUG
#define Assert(X)  assert(X)
#define CheckNull(X)  assert(X); if(!X) return;
#define CheckNullExp(X,Expr)  assert(X); if(!X) Expr;
#else
#define Assert(X)
#define CheckNull(X)  if(!X) return;
#define CheckNullExp(X,Expr)  assert(X); if(!X) Expr;
#endif

#endif // _HELIOS_ASSERT_H