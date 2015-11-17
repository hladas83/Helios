#include "Camera.h"
#include "../Entity.h"

namespace Helios
{
  //===================================================================================================

  ICamera::ICamera() :
    _lookAt(0,0,0),
    _eye(1,0,0),
    _up(0,1,0)
  {

  }

  //===================================================================================================

  Camera::Camera() :   
    _lastMouseX(-1), 
    _lastMouseY(-1), 
    _lastMouseW(-1),
    _lMouseDown(false), 
    _rMouseDown(false), 
    _cameraAngleX(0), 
    _cameraAngleY(0), 
    _cameraDistance(10),
    _lockCameraRelation(true)
  {
    UpdteCamera();
  }

  //------------------------------------------------------------------------------

  void Camera::UpdteCamera()
  {
    if(_lockCameraRelation && _target)
    {
      _eye = Vector3( 
        sin(_cameraAngleX) * cos(_cameraAngleY) * _cameraDistance ,
        sin(_cameraAngleY) * _cameraDistance, 
        cos(_cameraAngleX) * cos(_cameraAngleY) * _cameraDistance);
      _eye = _eye + _target->GetPosition<RenderState>();

      Vector3 aside = Vector3(_eye.Z(), 0.0f, -_eye.X()).Normalized();
      if(cos(_cameraAngleY) < 0) aside = aside * -1.0f;

      _up = _eye.Cross(aside).Normalized();
      _lookAt = _target->GetPosition<RenderState>();
    }
    else
    {
      _eye = Vector3( 
        sin(_cameraAngleX) * cos(_cameraAngleY) * _cameraDistance ,
        sin(_cameraAngleY) * _cameraDistance, 
        cos(_cameraAngleX) * cos(_cameraAngleY) * _cameraDistance);

      Vector3 aside = Vector3(_eye.Z(), 0.0f, -_eye.X()).Normalized();
      if(cos(_cameraAngleY) < 0) aside = aside * -1.0f;

      _up = _eye.Cross(aside).Normalized();

      if(_target) _lookAt = _target->GetPosition<RenderState>();
      else _lookAt  = Vector3( 0.0f, 0.0f, 0.0f );
    }
  }

  //------------------------------------------------------------------------------

  void Camera::MoveCamera(int mouseX, int mouseY)
  {
    if(_lastMouseX < 0 || _lastMouseY < 0)
    {  
      _lastMouseX = mouseX;
      _lastMouseY = mouseY;
    }
    else
    {
      short deltaX = mouseX - _lastMouseX;
      short deltaY = mouseY - _lastMouseY;

      _cameraAngleX += deltaX * 0.01f;
      _cameraAngleY += deltaY * 0.01f;

      _lastMouseX = mouseX;
      _lastMouseY = mouseY;
    }
  }

  //------------------------------------------------------------------------------

  void Camera::ZoomCamera(int mouseW)
  {
    _cameraDistance = max(_cameraDistance -mouseW * 0.01f, 0.1f);
  }

  //------------------------------------------------------------------------------

  //camera target
  void Camera::SetTarget(Entity *target) 
  {
    _target = target;
  };

  //------------------------------------------------------------------------------

  const Entity *Camera::GetTarget() const 
  {
    return _target;
  };

  //------------------------------------------------------------------------------



} // Helios namespace