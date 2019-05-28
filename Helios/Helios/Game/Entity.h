#pragma once

#ifndef _HELIOS_ENTITY
#define _HELIOS_ENTITY

//===================================================================================================
// included dependencies

#include "../../Elementary/Types.h"
#include "SpaceObjects/Components/MovementComponents/PlainSimulation.h"
#include "../Engine/Components/ComponentHolder.h"

#include <string>
#include <math.h>
#include <list>
#include <atomic>

namespace Helios
{

  //===================================================================================================
  // forward declared dependencies

  class PlainSimulation;
  class RenderObject;
  class UIBasicInfo;
  class OrbitersComponent;

  //===================================================================================================
  //basic object

  class Entity : public ComponentHolder, public OLinkBase<Entity>
  {
    typedef Component base;

  protected:
    //! simulation type
    Ref<PlainSimulation> _movementSimulation;
    //! object shape/shader/texture description
    std::vector<Ref<RenderObject>> _renderObjects;

    //! object scale
    float _size;

    //entity configuration
    WParamItem _entityConfig;
    HString _name;

  private:
    // initialize this class (not virtual, so it can be called from constructor)
    void InitClass();
    virtual void InitClass(ComponentHolder *parent, WParamItem &entityConfig) override { Assert(false); };

  public:
    Entity();
    Entity(Entity *parent, const HString &name);
    virtual ~Entity();

    //!return config class name
    const HString GetName() const {return _name;};

    //! object movement simulation class
    PlainSimulation *GetMovementSimulation() const;
    ObejctState *GetSimulationState() ;
    ObejctState *GetRenderVisualState();

    //! get object shape description
    Ref<RenderObject> GetRenderObject(unsigned int index) const;
    int GetRenderObjectCount() const;

    //!Set position (simulation state)
    void SetPosition(float x, float y, float z);
    void SetPosition(const Vector3 &position) ;
    
    //!Get position
    template<__ObejctStateTypes type> 
    Vector3 GetPosition() const 
    {
      if(_movementSimulation) return _movementSimulation->GetPosition<type>(); 
      return Vector3(0,0,0);
    };    

    //set object scale
    __forceinline float GetSize() {return _size;};
    __forceinline void SetSize(float size) {_size = size;};
      





    //! component Simulate loop
    virtual void Simulate(float deltaT) override;
    virtual void PostSimulate() override;
    //! component Draw thread method
    virtual void Draw() override;
    virtual void PostDraw() override;

    USE_CASTING(base);


  };

} //namespace

#endif // _HELIOS_ENTITY