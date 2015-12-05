#pragma once

//===================================================================================================
// included dependencies

#include "../../../common.h"
#include "../Engine/Components/ComponentHolder.h"
//===================================================================================================
// forward declared dependencies

namespace Helios
{

  class RenderObject;

  class EntityOrbitComponent : public ComponentHolder
  {
  private:
    Ref<RenderObject> _renderOrbitObject;

  public:
    EntityOrbitComponent(ComponentHolder *parent, WParamItem &entityConfig);
    ~EntityOrbitComponent();

    virtual void Draw() override;
  };

 

} //namespace
