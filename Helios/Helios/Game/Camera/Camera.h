#pragma once

#ifndef _HELIOS_CAMERA
#define _HELIOS_CAMERA

//===================================================================================================
// included dependencies

#include "../../common.h"

namespace Helios
{

  //===================================================================================================
  // forward declared dependencies

  class Entity;

  //===================================================================================================
  // ICamera class

  class ICamera : public RefCount, public OLinkBase<ICamera>
  {
  protected:
    Vector3 _lookAt, _eye, _up;

  public:
    ICamera();

    //-----------------------------------------------------------------------------
    //camera movement
    virtual void UpdteCamera() = 0;
    virtual void MoveCamera(int mouseX, int mouseY) = 0;
    virtual void ZoomCamera(int mouseW) = 0;

    const Vector3 GetLookAt()  const {return _lookAt;};
    const Vector3 GetPosition()  const {return _eye;};
    const Vector3 GetDirUp()  const {return _up;};

    //-----------------------------------------------------------------------------
    //camera target
    virtual void SetTarget(Entity *target)  = 0;
    virtual const Entity *GetTarget() const  = 0;
  };

  //===================================================================================================
  // Camera class

  class Camera : public ICamera
  {
  private:
    //mouse state
    short _lastMouseX; 
    short _lastMouseY;
    short _lastMouseW;
    bool _lMouseDown;
    bool _rMouseDown;

    //camera position settings
    float _cameraAngleX;
    float _cameraAngleY;
    float _cameraDistance;

    //camera target
    bool _lockCameraRelation; //lock camera distance from target
    OLink<Entity> _target;

  public:
    Camera();

    //-----------------------------------------------------------------------------
    //camera movement
    virtual void UpdteCamera() override;
    virtual void MoveCamera(int mouseX, int mouseY) override;
    virtual void ZoomCamera(int mouseW) override;

    //-----------------------------------------------------------------------------
    //camera target
    virtual void SetTarget(Entity *target) override;
    virtual const Entity *GetTarget() const override;

    //-----------------------------------------------------------------------------
    //camara target relation
    __forceinline void LockCameraRelation(bool lock) {_lockCameraRelation = lock;};
    __forceinline bool IsCameraRelationLocked() const {return _lockCameraRelation;};

  };

} // Helios namespace

#endif // _CAMERA