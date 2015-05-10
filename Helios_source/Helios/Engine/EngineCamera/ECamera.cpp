#include "ECamera.h"

namespace Helios
{

  //===================================================================================================

  ECamera::ECamera() 
  {
    _Eye = XMVectorSet( 10.0f, 0.0f, 0.0f, 0.0f );
    _Up =  XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
    _At =  XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );
  }

  //------------------------------------------------------------------------------

  void ECamera::SetCamera(const Vector3 &eye, const Vector3 &at, const Vector3 &up)
  {
    _Eye = XMVectorSet( eye.X(),eye.Y(), eye.Z(), 0.0f );
    _Up =  XMVectorSet( up.X(), up.Y(),  up.Z(),  0.0f );
    _At =  XMVectorSet( at.X(), at.Y(),  at.Z(),  0.0f );
  };

  //------------------------------------------------------------------------------

  void ECamera::GetCameraMatrix(XMMATRIX  &g_View) const
  {
    g_View = XMMatrixLookAtLH( _Eye, _At, _Up );
  }

  //------------------------------------------------------------------------------

  XMVECTOR ECamera::GetCameraPosition() const
  {
    return _Eye;
  }

} // Helios namespace