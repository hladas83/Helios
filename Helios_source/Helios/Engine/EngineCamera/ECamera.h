#pragma once

#ifndef _HELIOS_ENGINE_CAMERA
#define _HELIOS_ENGINE_CAMERA

//===================================================================================================
// included dependencies

#include "../../common.h"

namespace Helios
{
  //===================================================================================================
  // Camera class

  class ECamera : public RefCount, public OLinkBase<ECamera>
  {
  private:
    //camera settings
    XMVECTOR _Eye, _At, _Up;

  public:
    ECamera();

    //------------------------------------------------------------------------------ 
    void SetCamera(const Vector3 &eye, const Vector3 &at, const Vector3 &up);
    void GetCameraMatrix(XMMATRIX  &g_View) const;
    XMVECTOR GetCameraPosition() const;
  };

} // Helios namespace

#endif // _HELIOS_ENGINE_CAMERA